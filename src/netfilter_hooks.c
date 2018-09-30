#include <linux/module.h>
#include <linux/version.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>

static struct nf_hook_ops reg;
static DEFINE_MUTEX(nf_hook_mutex);
static DEFINE_MUTEX(module_lock);

#define nf_entry_dereference(e) \
	rcu_dereference_protected(e, lockdep_is_held(&nf_hook_mutex))

struct module *get_module_from_addr(unsigned long addr){
	return  __module_address(addr);
}

static void search_hooks(const struct nf_hook_entries *e){
	int i;
	unsigned long addr;
	struct module *mod;

	for (i = 0; i < e->num_hook_entries; i++){
		addr = (unsigned long)e->hooks[i].hook;
		mutex_lock(&module_lock);
		mod = get_module_from_addr(addr);
		if (mod){
			printk("Module [%s] controls a hook.\n", mod->name);
		} else {
			/* Attempt hidden discovery in the future */
		}
		mutex_unlock(&module_lock);
	}
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,16,0)

static struct nf_hook_entries __rcu **nf_hook_entry_head(struct net *net, int pf, unsigned int hooknum, struct net_device *dev){
	switch (pf){
	case NFPROTO_NETDEV:
		break;
#ifdef CONFIG_NETFILTER_FAMILY_ARP
	case NFPROTO_ARP:
		if (WARN_ON_ONCE(ARRAY_SIZE(net->nf.hooks_arp) <= hooknum))
			return NULL;
		return net->nf.hooks_arp + hooknum;
#endif
#ifdef CONFIG_NETFILTER_FAMILY_BRIDGE
	case NFPROTO_BRIDGE:
		if (WARN_ON_ONCE(ARRAY_SIZE(net->nf.hooks_bridge) <= hooknum))
			return NULL;
		return net->nf.hooks_bridge + hooknum;
#endif
	case NFPROTO_IPV4:
		if (WARN_ON_ONCE(ARRAY_SIZE(net->nf.hooks_ipv4) <= hooknum))
			return NULL;
		return net->nf.hooks_ipv4 + hooknum;
	case NFPROTO_IPV6:
		if (WARN_ON_ONCE(ARRAY_SIZE(net->nf.hooks_ipv6) <= hooknum))
			return NULL;
		return net->nf.hooks_ipv6 + hooknum;
#if IS_ENABLED(CONFIG_DECNET)
	case NFPROTO_DECNET:
		if (WARN_ON_ONCE(ARRAY_SIZE(net->nf.hooks_decnet) <= hooknum))
			return NULL;
		return net->nf.hooks_decnet + hooknum;
#endif
	default:
		WARN_ON_ONCE(1);
		return NULL;
	}

#ifdef CONFIG_NETFILTER_INGRESS
	if (hooknum == NF_NETDEV_INGRESS) {
		if (dev && dev_net(dev) == net)
			return &dev->nf_hooks_ingress;
	}
#endif
	WARN_ON_ONCE(1);
	return NULL;
}

int analyze_netfilter(void){
	struct nf_hook_entries *p;
	struct nf_hook_entries __rcu **pp;

	reg.hooknum = NF_INET_PRE_ROUTING;
	reg.pf = PF_INET;
	reg.priority = NF_IP_PRI_FIRST;

	pp = nf_hook_entry_head(&init_net, reg.pf, reg.hooknum, reg.dev);
	if (!pp)
		return -EINVAL;

	p = nf_entry_dereference(*pp);
	if (!p)
		return 0;

	search_hooks(p);
	return 0;
}

#elif LINUX_VERSION_CODE >= KERNEL_VERSION(4,14,0)

static struct nf_hook_entries __rcu **nf_hook_entry_head(struct net *net, const struct nf_hook_ops *reg){
	if (reg->pf != NFPROTO_NETDEV)
		return net->nf.hooks[reg->pf]+reg->hooknum;

#ifdef CONFIG_NETFILTER_INGRESS
	if (reg->hooknum == NF_NETDEV_INGRESS) {
		if (reg->dev && dev_net(reg->dev) == net)
			return &reg->dev->nf_hooks_ingress;
	}
#endif
	return NULL;
}

int analyze_netfilter(void){
	struct nf_hook_entries *p;
	struct nf_hook_entries __rcu **pp;

	reg.hooknum = NF_INET_PRE_ROUTING;
	reg.pf = PF_INET;
	reg.priority = NF_IP_PRI_FIRST;

	pp = nf_hook_entry_head(&init_net, &reg);
	if (!pp)
		return -EINVAL;

	p = nf_entry_dereference(*pp);
	if (!p)
		return 0;

	search_hooks(p);
	return 0;
}
#endif
