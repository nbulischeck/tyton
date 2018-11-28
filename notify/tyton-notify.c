#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <poll.h>
#include <libnotify/notify.h>
#include <systemd/sd-journal.h>

#ifndef SD_JOURNAL_SYSTEM
#define SD_JOURNAL_SYSTEM SD_JOURNAL_SYSTEM_ONLY
#endif

struct view { const char *s; size_t len; };

void destroy(char **args){
	int i = 0;
	while (args[i]){
		free(args[i++]);
	} free(args);
}

char **instantiate(struct view *result, size_t size){
	int i;
	char **strings = malloc((size+1) * sizeof(char *));
	for (i = 0; i < size; i++){
		strings[i] = malloc(((result+i)->len+1) * sizeof(char));
		strncpy(strings[i], (result+i)->s, (result+i)->len);
		strings[i][(result+i)->len] = '\0';
	}
	strings[size] = NULL;
	return strings;
}

size_t partition(struct view *result, size_t n,
					const char *str, const char *delim){
	char *startp = (char *)str, *endp;
	size_t l = strlen(delim), i = 0;

	while ((endp = strstr(startp, delim)) && (i < n)){
		size_t diff = endp - startp;
		if (diff){
			(result+i)->s = startp;
 			(result+i++)->len = diff;
		}
		startp = endp + l;
	}

	if ((*startp) && (i < n)){
		(result+i)->s = startp;
		(result+i++)->len = strlen(startp);
	}

	return (i > n) ? n : i;
}

char **parse_message(const char *message){
	size_t n = 1024, ret = 0;
	char **strings;
	struct view *result;

	result = malloc(n * sizeof(struct view));
	if (!result)
		return NULL;

	ret = partition(result, n, message, ": ");
	if (ret != 3){
		free(result);
		return NULL;
	}

	strings = instantiate(result, ret);

	free(result);

	return strings;
}

void notify_user(NotifyNotification *notification, const char *message){
	char *title, *type, **strings;

	title = "Tyton Kernel Message";
	strings = parse_message(message);

	if (strcmp(strings[1], "ALERT") == 0){
		type = "dialog-information";
	} else if (strcmp(strings[1], "WARNING") == 0){
		type = "dialog-warning";
	} else if (strcmp(strings[1], "ERROR") == 0){
		type = "dialog-error";
	} else {
		goto exit;
	}

	notify_notification_update(notification, title, strings[2], type);
	notify_notification_show(notification, NULL);

exit:
	destroy(strings);
}

int poll_journal(sd_journal *j){
	int msec;
	uint64_t t, n;
	struct timespec ts;
	struct pollfd pollfd;

	if (sd_journal_get_timeout(j, &t) < 0)
		return -1;

	if (t == (uint64_t) -1){
		msec = -1;
	} else {
		if (clock_gettime(CLOCK_MONOTONIC, &ts) < 0)
			return -1;
		n = (uint64_t) ts.tv_sec * 1000000 + ts.tv_nsec / 1000;
		msec = t > n ? (int) ((t - n + 999) / 1000) : 0;
	}

	pollfd.fd = sd_journal_get_fd(j);
	pollfd.events = sd_journal_get_events(j);
	poll(&pollfd, 1, msec);

	return sd_journal_process(j);
}

int main(int argc, char **argv){
	int r;
    size_t l;
    const void *d;
	sd_journal *j;
	NotifyNotification *notification;

	if (daemon(0, 0) < 0){
		fprintf(stderr, "Failed to daemonize.\n");
		return 1;
	}

	notify_init("Tyton Kernel Message");
	notification = notify_notification_new(NULL, NULL, NULL);

	if (sd_journal_open(&j, SD_JOURNAL_SYSTEM) < 0)
		return 1;

	if (sd_journal_get_fd(j) < 0)
		return 1;

	if (sd_journal_seek_tail(j) < 0)
		return 1;

	if (sd_journal_previous(j) < 0)
		return 1;

	while ((r = sd_journal_next(j)) >= 0){
		if (r == 0){
			if (poll_journal(j) < 0)
				break;
			continue;
		}

		if (sd_journal_get_data(j, "MESSAGE", &d, &l) < 0)
			return 1;

		if (strstr(d, "tyton"))
			notify_user(notification, d);
	}

	sd_journal_close(j);
	g_object_unref(G_OBJECT(notification));
	notify_uninit();

	return 0;
}
