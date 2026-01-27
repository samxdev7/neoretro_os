/* Comp.h - Components for OS Interfaces */
#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <stdio.h>
#include <conio.h>
#include <graphics.h>
#include <stdlib.h>
#include <dos.h>
#include <string.h>
#include <gphadmin.h>
#include <dynamic.h>
#include <mouse256.h>

#define CENTER_X(COMP) ((COMP)->x1 + ((COMP)->x2 - (COMP)->x1) / 2)
#define CENTER_Y(COMP) ((COMP)->y1 + ((COMP)->y2 - (COMP)->y1) / 2)

typedef struct {
    unsigned char available;
    unsigned char bg_c;
    unsigned char text_c;
    unsigned char border_c;
    unsigned char status;
} HoverStyle;

typedef struct
{
	short x1, y1;
    short x2, y2;
    unsigned char bg_c;
    unsigned char text_c;
    unsigned char border_c;
    HoverStyle hover;
} Component;

typedef enum {
    HOVER_NOT_AVAILABLE,
    HOVER_AVAILABLE
} HoverAvailable;

typedef enum {
    HOVER_NOT_ACTIVE,
    HOVER_ACTIVE
} HoverStatus;

void constructor_comp(Component *comp, short x1, short y1,
                      short x2, short y2,
                      unsigned char bg_c,
                      unsigned char text_c,
                      unsigned char border_c,
                      unsigned char available) {
    comp -> x1 = x1;
    comp -> y1 = y1;
    comp -> x2 = x2;
    comp -> y2 = y2;
    comp -> bg_c = bg_c;
    comp -> text_c = text_c;
    comp -> border_c = border_c;
    comp -> hover.available = available;
}

void constructor_comp_hover(
    Component *comp,
	unsigned char bg_c,
	unsigned char text_c,
	unsigned char border_c
) {
    if (comp->hover.available == HOVER_AVAILABLE) {
        comp -> hover.bg_c = bg_c;
        comp -> hover.text_c = text_c;
        comp -> hover.border_c = border_c;
        comp -> hover.status = HOVER_NOT_ACTIVE; 
    }
}

void render_comp(Component *comp) {
    setfillstyle(SOLID_FILL, comp->bg_c);
    bar(comp->x1, comp->y1, comp->x2, comp->y2);
    
    setcolor(comp -> border_c);
    rectangle(comp->x1, comp->y1, comp->x2, comp->y2);
}

void render_comp_text(Component *comp, char *text) {
	short tx, ty;
    
    render_comp(comp);

    tx = CENTER_X(comp) - textwidth(text) / 2;
	ty = CENTER_Y(comp) - textheight(text) / 2 - 2;
    
    setcolor(comp -> color);
    
    if (textwidth(text) >= comp->x2 - comp->x1)
        fit_large_text_comp(comp, text, &tx, &ty);
    else
	    outtextxy(tx, ty, text);
}

void render_comp_hover(Component *comp, void (*special_render)(Component*)) {
    short temp_bg_c, temp_text_c, temp_border_c;

    temp_bg_c = comp->bg_c;
    temp_text_c = comp->text_c;
    temp_border_c = comp->border_c;
    
    comp->bg_c     = comp->hover.bg_c;
    comp->text_c   = comp->hover.text_c;
    comp->border_c = comp->hover.border_c;
    
    if (special_render != NULL)
        special_render(comp);
    else  
        render_comp(comp);
    
    comp->bg_c     = temp_bg_c;
    comp->text_c   = temp_text_c;
    comp->border_c = temp_border_c;
}

void handle_comp_hover(Component *comp, short mx, short my, 
                       void (*special_render)(Component*)) {
    HoverStatus curr_hover;
    HoverStatus prev_hover;

    if (comp -> hover.available != HOVER_AVAILABLE) 
        return;
    
    curr_hover = mouse_over_comp(comp, mx, my) ? 
                    HOVER_ACTIVE : HOVER_NOT_ACTIVE;
    prev_hover = comp -> hover.status;

    if (curr_hover == prev_hover) 
        return;
    mhide();
        
    if (curr_hover == HOVER_ACTIVE)
        render_comp_hover(comp, special_render);

    else {
        if (special_render != NULL)
            special_render(comp);
        else
            render_comp(comp);
    }
    mshow();
    comp -> hover.status = curr_hover;
}

void handle_comp_hover_text(Component *comp, short mx, short my, char *text)
{
    HoverStatus curr_hover;
    HoverStatus prev_hover;
    short tx, ty;
    
    if (comp -> hover.available != HOVER_AVAILABLE) 
        return;
    
    curr_hover = mouse_over_comp(comp, mx, my) ? HOVER_ACTIVE : HOVER_NOT_ACTIVE;
    prev_hover = comp -> hover.status;
    
    if (curr_hover == prev_hover) 
        return;
    
    tx = CENTER_X(comp) - textwidth(text) / 2;
	ty = CENTER_Y(comp) - textheight(text) / 2;

    mhide();
        
    if (curr_hover == HOVER_ACTIVE) {
        render_comp_hover(comp, NULL);

        setcolor(comp->hover.color);
		if (textwidth(text) >= comp->x2 - comp->x1)
            fit_large_text_comp(comp, text, &tx, &ty);
        else
            outtextxy(tx, ty, text);
        setcolor(comp->color);
    } 
    
    else {
        render_comp(comp);

        setcolor(comp->color);
		if (textwidth(text) >= comp->x2 - comp->x1)
			fit_large_text_comp(comp, text, &tx, &ty);
        else
            outtextxy(tx, ty, text);
    }
    mshow();
    
    comp -> hover.status = curr_hover;
}

short mouse_over_comp(Component *comp, short mouse_x, short mouse_y) {
    return (mouse_x >= comp->x1 && mouse_x <= comp->x2 &&
            mouse_y >= comp->y1 && mouse_y <= comp->y2);
}

short detect_click_comp(Component *comp, short mouse_x, short mouse_y) {
	return mouse_over_comp(comp, mouse_x, mouse_y) && (mclick() == 1);
}

void fit_large_text_comp(Component *comp, char *text, short *tx, short *ty)
{
    short len_x;
    short spacing_y = textheight(text) / 2;
    short tmp_size = 0;

    short letter = 0, last_space = 0;
    char processed_letter[2], *tmp_text;

    tmp_text = (char *) malloc(sizeof(text));
    if (tmp_text == NULL)
        return;

    memcpy(tmp_text, 0, sizeof(text));
    processed_letter[1] = '\0';
    
    len_x = (comp->x2 - comp->x1);

    do {
        processed_letter[0] = text[letter];

        if (text[letter] == ' ') 
            last_space = letter;

        tmp_size += textwidth(processed_letter);
        letter++;
    } while (tmp_size - 3 < len_x || text[letter] != '\0');

    letter = 0;
    while (letter < last_space) {
        tmp_text[letter] = text[letter];
        letter++;
    }
    tmp_text[letter] = 0;

    *tx = (len_x - textwidth(tmp_text)) / 2 + comp->x1;

    outtextxy(*tx, *ty - spacing_y, tmp_text)
    memcpy(tmp_text, 0, sizeof(text));

    letter = last_space;
    while (text[letter] != '\0')
    {
        tmp_text[letter - last_space] = text[letter + 1];
        letter++;
    }

    *tx = (len_x - textwidth(tmp_text)) / 2 + comp->x1;
    outtextxy(*tx, *ty + spacing_y, tmp_text);
    free(tmp_text);
}
#endif  