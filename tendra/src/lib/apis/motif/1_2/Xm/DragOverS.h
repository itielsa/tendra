/*
    COPYRIGHT NOTICE

    This program is the proprietary property of IXI Ltd, a subsidiary
    of the Santa Cruz Operation (SCO). Use, reproduction, production
    of amended versions and/or transfer of this program is permitted
    PROVIDED THAT:

    (a)  This legend be preserved on any such reproduction and amended
         version.

    (b)  Any recipient of such reproduction or amended version accept
         the conditions set out in this legend.

    IXI accepts no liability whatsoever in relation to any use to
    which this program may be put and gives no warranty as to the
    program's suitability for any purpose.

    All rights reserved.

    Copyright (c) 1995, 1996

*/




/* SCO CID (IXI) DragOverS.h,v 1.1 1996/08/08 14:12:38 wendland Exp */

+USE "motif/1_2", "Xm/Xm.h";
+USE "motif/1_2", "Xm/DragC.h";
+USE "motif/1_2", "Xm/DragIcon.h";

+TYPE (struct) _XmDragOverShellRec;
+TYPE (struct) _XmDragOverShellClassRec;
+TYPEDEF _XmDragOverShellRec *XmDragOverShellWidget;
+TYPEDEF _XmDragOverShellClassRec *XmDragOverShellWidgetClass;

+EXP lvalue WidgetClass xmDragOverShellWidgetClass;

