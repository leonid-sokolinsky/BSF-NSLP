/*==============================================================================
Project: Bulk Synchronous Farm (BSF)
Theme: BSF Skeleton
Module: BSF-Types.h (BSF Types)
Prefix: BT
Author: Leonid B. Sokolinsky
Creation Date: 30.05.2017
==============================================================================*/
#pragma once
#include "BSF-Include.h"

struct BT_order_T {  
	char exit;		// true, if worker must stop
	int index;		// index of the first sublist element
	int length;		// length of sublist
};