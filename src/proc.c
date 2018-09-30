#include <linux/fs.h>

#include "core.h"
#include "util.h"

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,11,0)
#include <linux/sched/signal.h>
#else
#include <linux/sched.h>
#endif

struct linux_dirent {
	unsigned long 	d_ino;
	unsigned long 	d_off;
	unsigned short 	d_namlen;
	unsigned long 	d_type;
	char 			d_name[];
};

struct readdir_data {
	struct dir_context 	ctx;
	char 				*dirent;
	size_t 				used;
	int 				full;
	int 				entries;
};

static int filldir_fn(struct dir_context *ctx, const char *name, int namlen, loff_t offset, u64 ino, unsigned int type){
	struct readdir_data *buf;
	struct linux_dirent *d;
	unsigned int reclen;

	buf = container_of(ctx, struct readdir_data, ctx);
	d = (void *)(buf->dirent + buf->used);

	reclen = ALIGN(sizeof(struct linux_dirent) + namlen, sizeof(u64));
	if (buf->used + reclen > PAGE_SIZE) {
		buf->full = 1;
		return -EINVAL;
	}

	d->d_ino = ino;
	d->d_off = offset;
	d->d_namlen = namlen;
	d->d_type = type;
	memcpy(d->d_name, name, namlen);
	buf->used += reclen;
	buf->entries++;

	return 0;
}

void analyze_processes(void){
	int size, error;
	struct file *fp;
	struct task_struct *task;
	struct linux_dirent *d;
	struct readdir_data buf = {
		.ctx.actor = filldir_fn,
		.dirent = (void *)__get_free_page(GFP_KERNEL),
		.entries = 0
	};

	fp = filp_open("/proc", O_RDONLY, S_IRUSR);
	if (IS_ERR(fp)){
		printk(KERN_ERR "[TYTON]: Failed to open /proc.");
		return;
	}

	while (1){
		unsigned int reclen;
		buf.used = 0;
		buf.full = 0;

		error = iterate_dir(fp, &buf.ctx);
		if (buf.full)
			error = 0;

		if (error < 0)
			break;

		size = buf.used;

		if (!size)
			break;

		d = (struct linux_dirent *)buf.dirent;
		while (size > 0){
			reclen = ALIGN(sizeof(*d) + d->d_namlen, sizeof(u64));
			d = (struct linux_dirent *)((char *)d + reclen);
			size -= reclen;
		}

		if (size > 0)
			break;
	}

	free_page((unsigned long)(buf.dirent));

	for_each_process(task){
		printk("[%d] %s\n", task->pid, task->comm);
	}

	return;
}
