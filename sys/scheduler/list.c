/*
 *
 *  This file is part of sbunix.
 *        sbunix is a simple linux implementation, basically, it is
 *  an academic project of CSE506 of Stony Brook University in Spring 
 *  2015. For more details, please refer to README.md.
 *
 *  Copyright (C) 2015 Dongju Ok   <dongju@stonybrook.edu,
 *                                  yardbirds79@gmail.com>
 *  Copyright (C) 2015 Yigong Wang <yigwang@cs.stonybrook.edu>
 * 
 *
 *  sbunix is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 * 
 *  sbunix is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License
 *  along with sbunix.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include <sys/sched/sched.h>
#include <sys/sched/list.h>
#include <sys/mem.h>


void* remove_list(LIST* list, uint64_t id);


void add_list_to_tail(LIST* list, void* task)
{
	LISTLINK* link;
    
	link = ( LISTLINK* )task;
	link->next = NULL;
    
	// if list is empny, 
	if(list->header == NULL)
	{
		list->header = task;
		list->tail = task;
		list->item_count = 1;

		return ;
	}
	// update tail of list and link poins to next task
	link = (LISTLINK*)list->tail;
	link->next = task;

	// update 
	list->tail = task;
	list->item_count++;
}

void* remove_list_from_header(LIST* list)
{
	LISTLINK* link;
    
	if(list->item_count == 0 )
	{
		return NULL;
	}

	// remove header and return
	link = (LISTLINK*)list->header;
	return remove_list(list, link->id);
	//return remove_list(list, 0);
}

void* remove_list(LIST* list, uint64_t id)
{
	LISTLINK *link;
	LISTLINK *previous_link;

	previous_link = (LISTLINK*)list->header;

	//TODO removed or still exist about id
	for(link = previous_link; link != NULL; link = link->next)
	{
		if(link->id == id)
		{
			if((link == list->header) &&
			   (link == list->tail))
			{
				list->header = NULL;
				list->tail = NULL;
			}
			else if(link == list->header)
			{
				list->header = link->next;

			}
			else if(link == list->tail)
			{
				list->tail = previous_link;
			}
			else
			{
				previous_link->next = link->next;
			}
			
			list->item_count--;
			return link;

		}
		previous_link = link;

	}
	return	NULL;

}
