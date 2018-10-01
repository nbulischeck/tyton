#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>

#include "core.h"
#include "util.h"
#include "module_list.h"

#define nf_entry_dereference(e) \
	rcu_dereference_protected(e, lockdep_is_held(&nf_hook_mutex))

static void search_hooks(const struct nf_hook_entries *e){
	int i;
	unsigned long addr;
	struct module *mod;

	for (i = 0; i < e->num_hook_entries; i++){
		addr = (unsigned long)e->hooks[i].hook;
		mutex_lock(&module_mutex);
		mod = get_module_from_addr(addr);
		if (mod){
			printk(KERN_ALERT "[TYTON] Module [%s] controls a Netfilter hook.\n", mod->name);
		} else {
			find_hidden_modules(addr);
		}
		mutex_unlock(&module_mutex);
	}
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,16,0)

static struct nf_hook_entries __rcu **nf_hook_entry_head(struct net *net, int pf, unsigned int hooknum, struct net_device *dev){
	switch (pf){
		case NFPROTO_NETDEV:
			break;
		case NFPROTO_ARP:
			if (ARRAY_SIZE(net->nf.hooks_arp) <= hooknum)
				return NULL;
			return net->nf.hooks_arp + hooknum;
		case NFPROTO_BRIDGE:
			if (ARRAY_SIZE(net->nf.hooks_bridge) <= hooknum)
				return NULL;
			return net->nf.hooks_bridge + hooknum;
		case NFPROTO_IPV4:
			if (ARRAY_SIZE(net->nf.hooks_ipv4) <= hooknum)
				return NULL;
			return net->nf.hooks_ipv4 + hooknum;
		case NFPROTO_IPV6:
			if (ARRAY_SIZE(net->nf.hooks_ipv6) <= hooknum)
				return NULL;
			return net->nf.hooks_ipv6 + hooknum;
#if IS_ENABLED(CONFIG_DECNET)
		case NFPROTO_DECNET:
			if (ARRAY_SIZE(net->nf.hooks_decnet) <= hooknum)
				return NULL;
			return net->nf.hooks_decnet + hooknum;
#endif
		default:
			return NULL;
	}
	if (hooknum == NF_NETDEV_INGRESS){
		if (dev && dev_net(dev) == net)
			return &dev->nf_hooks_ingress;
	}
	return NULL;
}

#elif LINUX_VERSION_CODE >= KERNEL_VERSION(4,14,0)

static struct nf_hook_entries __rcu **nf_hook_entry_head(struct net *net, int pf, unsigned int hooknum, struct net_device *dev){
	if (pf != NFPROTO_NETDEV)
		return net->nf.hooks[pf]+hooknum;
	if (hooknum == NF_NETDEV_INGRESS){
		if (dev && dev_net(dev) == net)
			return &dev->nf_hooks_ingress;
	}
	return NULL;
}

#endif

int analyze_netfilter(void){
	int i, j;
	struct nf_hook_entries *p;
	struct nf_hook_entries __rcu **pp;

	printk(KERN_INFO "[TYTON] Analyzing Netfilter Hooks\n");

	for (i = 0; i < NFPROTO_NUMPROTO; i++){
		for (j = 0; j < NF_MAX_HOOKS; j++){
			pp = nf_hook_entry_head(&init_net, i, j, NULL);
			if (!pp)
				return -EINVAL;

			p = nf_entry_dereference(*pp);
			if (!p)
				continue;

			search_hooks(p);
		}
	}

	return 0;
}
