#include <errno.h>
#include <linux/nl80211.h>
#include <net/if.h>

#include <netlink/genl/genl.h>
#include <netlink/genl/family.h>
#include <netlink/genl/ctrl.h>
#include <netlink/msg.h>
#include <netlink/attr.h>

#include "iw.h"

static int handle_name(struct nl80211_state *state,
		       struct nl_msg *msg,
		       int argc, char **argv)
{
	int err = -ENOMEM;
	struct nl_cb *cb;

	if (argc != 1)
		return 1;

	NLA_PUT_STRING(msg, NL80211_ATTR_WIPHY_NAME, *argv);

	cb = nl_cb_alloc(NL_CB_CUSTOM);
	if (!cb)
		goto out;

	if ((err = nl_send_auto_complete(state->nl_handle, msg)) < 0)
		goto out;

	err = nl_wait_for_ack(state->nl_handle);

 out:
	nl_cb_put(cb);
 nla_put_failure:
	return err;
}
COMMAND(set, name, "<new name>", NL80211_CMD_SET_WIPHY, 0, CIB_PHY, handle_name);
