/* This file is part of GNU cflow
   Copyright (C) 1997,2005 Sergey Poznyakoff
 
   GNU cflow is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
 
   GNU cflow is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GNU cflow; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA  */

#include <cflow.h>

/* Print current tree level
 */
static void
print_level(int lev, int last)
{
    int i;

    if (print_levels)
	fprintf(outfile, "%4d ", lev);
    fprintf(outfile, "%s", level_begin);
    for (i = 0; i < lev; i++) 
	fprintf(outfile, "%s", level_indent[ level_mark[i] ]);
    fprintf(outfile, "%s", level_end[last]);
}

void
print_function_name(Symbol *sym, int has_subtree)
{
    fprintf(outfile, "%s()", sym->name);
    if (sym->v.func.type)
	fprintf(outfile, " <%s at %s:%d>",
		sym->v.func.type,
		sym->v.func.source,
		sym->v.func.def_line);
    if (sym->active) {
	 fprintf(outfile, " (recursive: see %d)", sym->active-1);
	 return;
    }
    if (sym->v.func.recursive)
	 fprintf(outfile, " (R)");
    if (!print_as_tree && has_subtree)
	 fprintf(outfile, ":");
}


static void
print_symbol(FILE *outfile, int line, struct output_symbol *s)
{
     if (s->direct) {
	  print_level(s->level, s->last);
	  print_function_name(s->sym, s->sym->v.func.callee != NULL);
	  
	  if (brief_listing) {
	       if (s->sym->expand_line) 
		    fprintf(outfile, " [see %d]", s->sym->expand_line);
	       else if (s->sym->v.func.callee)
		    s->sym->expand_line = line;
	  }
     } else {
	  print_level(s->level, s->last);
	  print_function_name(s->sym, s->sym->v.func.caller != NULL);
     }
}

void
gnu_output_handler(cflow_output_command cmd,
		   FILE *outfile, int line,
		   void *data, void *handler_data)
{
     switch (cmd) {
     case cflow_output_begin:
     case cflow_output_end:
	  break;
     case cflow_output_newline:
     case cflow_output_separator:
	  fprintf(outfile, "\n");
	  break;
     case cflow_output_text:
	  fprintf(outfile, "%s\n", data);
	  break;
     case cflow_output_symbol:
	  print_symbol(outfile, line, data);
     }
}