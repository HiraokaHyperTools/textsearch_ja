/*-------------------------------------------------------------------------
 *
 * nodeSort.h
 *
 *
 *
 * Portions Copyright (c) 1996-2006, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $PostgreSQL: pgsql/src/include/executor/nodeSort.h,v 1.22 2006/03/05 15:58:56 momjian Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef NODESORT_H
#define NODESORT_H

#include "nodes/execnodes.h"

extern int	ExecCountSlotsSort(Sort *node);
extern SortState *ExecInitSort(Sort *node, EState *estate, int eflags);
extern TupleTableSlot *ExecSort(SortState *node);
extern void ExecEndSort(SortState *node);
extern void ExecSortMarkPos(SortState *node);
extern void ExecSortRestrPos(SortState *node);
extern void ExecReScanSort(SortState *node, ExprContext *exprCtxt);

#endif   /* NODESORT_H */
