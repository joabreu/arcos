/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#ifndef __ARCOS_LIST_H__
#define __ARCOS_LIST_H__

#include <arcOS/kernel.h>

struct list_head {
	struct list_head *next, *prev;
};

#define LIST_HEAD_INIT(n)	{ &(n), &(n) }
#define LIST_HEAD(n) \
	struct list_head (n) = LIST_HEAD_INIT(n)

static inline void INIT_LIST_HEAD(struct list_head *list)
{
	list->next = list;
	list->prev = list;
}

static inline void __list_add(struct list_head *new, struct list_head *prev,
		struct list_head *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}

static inline void list_add(struct list_head *new, struct list_head *head)
{
	__list_add(new, head, head->next);
}

static inline void list_add_tail(struct list_head *new, struct list_head *head)
{
	__list_add(new, head->prev, head);
}

static inline void __list_del(struct list_head *prev, struct list_head *next)
{
	next->prev = prev;
}

static inline void __list_del_entry(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
}

static inline void list_del(struct list_head *entry)
{
	__list_del_entry(entry);
}

static inline void list_del_init(struct list_head *entry)
{
	__list_del_entry(entry);
	INIT_LIST_HEAD(entry);
}

static inline int list_empty(const struct list_head *head)
{
	return head->next == head;
}

#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)
#define list_first_entry(ptr, type, member) \
	container_of((ptr)->next, type, member)
#define list_last_entry(ptr, type, member) \
	container_of((ptr)->prev, type, member)
#define list_first_entry_or_null(ptr, type, member)	({ \
	struct list_head *__head = (ptr); \
	struct list_head *__pos = __head->next; \
	__pos != __head ? list_entry(__pos, type, member) : NULL; \
})
#define list_next_entry(pos, member) \
	list_entry((pos)->member.next, typeof(*(pos)), member)
#define list_prev_entry(pos, member) \
	list_entry((pos)->member.prev, typeof(*(pos)), member)
#define list_for_each(pos, head) \
	for ((pos) = (head)->next; (pos) != (head); (pos) = (pos)->next)
#define list_for_each_prev(pos, head) \
	for ((pos) = (head)->prev; (pos) != (head); (pos) = (pos)->prev)
#define list_for_each_safe(pos, tmp, head) \
	for ((pos) = (head)->next, (tmp) = (pos)->next; (pos) != (head); \
		(pos) = (tmp), (tmp) = (tmp)->next)
#define list_for_each_prev_safe(pos, tmp, head) \
	for ((pos) = (head)->prev, (tmp) = (pos)->prev; (pos) != (head); \
		(pos) = (tmp), (tmp) = (tmp)->prev)
#define list_for_each_entry(pos, head, member) \
	for ((pos) = list_first_entry(head, typeof(*(pos)), member); \
		&(pos)->member != (head); \
		(pos) = list_next_entry(pos, member))
#define list_for_each_entry_reverse(pos, head, member) \
	for ((pos) = list_last_entry(head, typeof(*(pos)), member); \
		&(pos)->member != (head); \
		(pos) = list_prev_entry(pos, member))
#define list_for_each_entry_safe(pos, tmp, head, member) \
	for ((pos) = list_first_entry(head, typeof(*(pos)), member), \
		(tmp) = list_next_entry(pos, member); \
		&(pos)->member != (head); \
		(pos) = (tmp), (tmp) = list_next_entry(tmp, member))
#define list_for_each_entry_safe_reverse(pos, tmp, head, member) \
	for ((pos) = list_last_entry(head, typeof(*(pos)), member), \
		(tmp) = list_prev_entry(pos, member); \
		&(pos)->member != (head); \
		(pos) = (tmp), (tmp) = list_prev_entry(tmp, member))

#endif

