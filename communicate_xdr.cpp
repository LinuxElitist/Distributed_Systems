/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "communicate.h"

bool_t
xdr_join_1_argument (XDR *xdrs, join_1_argument *objp)
{
	 if (!xdr_string (xdrs, &objp->arg1, ~0))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->arg2))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_leave_1_argument (XDR *xdrs, leave_1_argument *objp)
{
	 if (!xdr_string (xdrs, &objp->arg1, ~0))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->arg2))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_subscribe_1_argument (XDR *xdrs, subscribe_1_argument *objp)
{
	 if (!xdr_string (xdrs, &objp->arg1, ~0))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->arg2))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->arg3, ~0))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_unsubscribe_1_argument (XDR *xdrs, unsubscribe_1_argument *objp)
{
	 if (!xdr_string (xdrs, &objp->arg1, ~0))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->arg2))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->arg3, ~0))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_publish_1_argument (XDR *xdrs, publish_1_argument *objp)
{
	 if (!xdr_string (xdrs, &objp->arg1, ~0))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->arg2, ~0))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->arg3))
		 return FALSE;
	return TRUE;
}
