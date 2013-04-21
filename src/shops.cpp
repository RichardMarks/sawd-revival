
/*
	SAWD Small ASCII Walk-around Demo (C) Copyright 2008, CCPS Solutions
	
	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc.,
	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <cstdarg>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "clib.h"

#include "sawd.h"


// SHOP DESIGN version 3.6

// the form that is shown when you enter a shop
int shop_form_a(void* d)
{
	char cursor_icon = (char)31;
	int cursor_x = 0;

	// the three buttons
	int btn[3][4] =
	{
		{4, 6, 7, 3},
		{12, 6, 8, 3},
		{21, 6, 8, 3}
	};
	char* btn_caption[3] = {"BUY","SELL","EXIT"};

	shopkeeper* keeper = (shopkeeper*)d;

	if (keeper->is_magic_shop)
	{
		if (player->chr_class != 3)
		{
			cl->show_message(3,"","You are not a Mage!","");
			return SHOP_OPTION_EXIT;
		}
	}

	char* shop_name = keeper->name;

	int box[] = {40 - (38 / 2), (25 / 2) - (14 / 2), 38, 14};
	int x = box[0] + 2;
	int y = box[1] + 2;
	int caption_x = x - 2 + (box[2] / 2) - strlen(shop_name) / 2;
	char* msg = "What would you like to do?";
	int msg_x = x - 2 + (box[2] / 2) - strlen(msg) / 2;

	bool close_box = false;

	// draw the form
	clibwindow* wnd = cl->open_window(box[0], box[1], box[2], box[3]);

	// draw the caption and the message
	cl->outchars(caption_x, y, shop_name);
	cl->outchars(msg_x, y + 2, msg);

	// draw the buttons
	{for(int n = 0; n < 3; n++)
	{
		//draw_message_box(x + btn[n][0], y + btn[n][1], btn[n][2], btn[n][3]);
		//cl->outchars(x + btn[n][0] + 2, y + btn[n][1] + 1, btn_caption[n]);

		cl->draw_button(x + btn[n][0], y + btn[n][1], btn[n][2], btn[n][3], btn_caption[n]);

		cl->outchar(x + btn[n][0]+3, y + 4, ' ');

	}
	}

	int form_result = SHOP_OPTION_EXIT;
	while(!close_box)
	{
		// clear the previous cursors
		{for(int n = 0; n < 3; n++){cl->outchar(x + btn[n][0]+3, y + 4, ' ');}}

		// draw the cursors in the right location
		if (cursor_x == 0) // BUY
		{
			cl->outchar(x + btn[0][0]+3, y + 4, cursor_icon);
		}
		else if (cursor_x == 1) // SELL
		{
			cl->outchar(x + btn[1][0]+3, y + 4, cursor_icon);
		}
		else if (cursor_x == 2) // EXIT
		{
			cl->outchar(x + btn[2][0]+3, y + 4, cursor_icon);
		}

		int k = cl->get_key();
		
		switch (k)
		{
		case VK_LEFT:
			{
				if (cursor_x > 0) 
					cursor_x--;
				else if (cursor_x <= 0)
					cursor_x = 2;
			} break;
		case VK_RIGHT:
			{
				if (cursor_x < 2) 
					cursor_x++;
				else if (cursor_x >= 2)
					cursor_x = 0;
			} break;

		case VK_BACK:
		case VK_ESCAPE:
			{
				form_result = SHOP_OPTION_EXIT;
				close_box = true;
			} break;

		case VK_SPACE:
		case VK_RETURN:
			{
				if (cursor_x == 0) // BUY
				{
					form_result = SHOP_OPTION_BUY;
				}
				else if (cursor_x == 1) // SELL
				{
					form_result = SHOP_OPTION_SELL;
				}
				else if (cursor_x == 2) // EXIT
				{
					form_result = SHOP_OPTION_EXIT;
				}
				close_box = true;
			} break;
		}
	}

	cl->close_window(wnd);
	return form_result;
}

void draw_shop_ext()
{
	char msg[32];
	sprintf_s(msg, 32, "$%d", player->gold);
	cl->draw_button(60,1,19,3,msg);
}

void hide_shop_ext()
{
	cl->draw_button(60,1,19,3,"",0,0);
}

// the form that is shown when you are buying items
int shop_form_b(void* d)
{
	int form_result = SHOP_OPTION_CANCEL;

	shopkeeper* keeper	= (shopkeeper*)d;
	int num_items = 0;

	if (keeper->is_magic_shop)
	{
		num_items		= (int)keeper->spells.size();
	}
	else
	{
		num_items		= (int)keeper->items.size();
	}

	int* item_qty		= new int [num_items];

	char btn_cursor_icon	= (unsigned char)31;
	char itm_cursor_icon_l	= (unsigned char)17;
	char itm_cursor_icon_r	= (unsigned char)16;

	int cursor_x = 0;
	int cursor_y = 0;

	
	int btn[2][4] = { {3, 15, 10, 3},	{21, 15, 10, 3} };
	char* btn_caption[2] = {"ACCEPT", "CANCEL"};
	
	

	
	for (int i = 0; i < num_items; i++) { item_qty[i] = 0; }

	int box[]		= {40 - (38 / 2), (25 / 2) - (22 / 2), 38, 22};
	int x			= box[0] + 2;
	int y			= box[1] + 2;
	int caption_x	= x - 2 + (box[2] / 2) - strlen(keeper->name) / 2;
	char* msg		= "What would you like to buy?";
	int msg_x		= x - 2 + (box[2] / 2) - strlen(msg) / 2;

	bool close_box	= false;

	clibwindow* wnd = cl->open_window(box[0], box[1], box[2], box[3]);
	
	cl->outchars(caption_x, y, keeper->name);
	cl->outchars(msg_x, y + 2, msg);

	// draw the available items
	{
		if (keeper->is_magic_shop)
		{
			for (unsigned int i = 0; i < keeper->spells.size(); i++)
			{
				cl->outchars(x,       y + (4 + i), keeper->spells.at(i)->name);
				cl->outcharsf(x + 20, y + (4 + i), "$%d", keeper->spells.at(i)->cost);
			}
		}
		else
		{
			for (unsigned int i = 0; i < keeper->items.size(); i++)
			{
				cl->outchars(x,       y + (4 + i), keeper->items.at(i)->name);
				cl->outcharsf(x + 20, y + (4 + i), "$%d", keeper->items.at(i)->cost);
			}
		}
	}

	// draw the buttons
	{
		for(int n = 0; n < 2; n++)
		{
			cl->draw_button(x + btn[n][0], y + btn[n][1], btn[n][2], btn[n][3], btn_caption[n]);
		}
	}

	draw_shop_ext();

	while(!close_box)
	{

		// draw the quantities
		{
			for (int n = 0; n < num_items; n++)
			{
				cl->outchars(x+28, y + (4 + n), "x   "); // clear old
				cl->outcharsf(x+28, y + (4 + n), "x%d", item_qty[n]);
			}
		}

		// clear all cursor positions
		{for(int n=0;n<num_items;n++){cl->outchar(x+26,y+4+n,' ');cl->outchar(x+33,y+4+n,' ');}
		cl->outchar(x + btn[0][0] + 3, y + 14, ' ');
		cl->outchar(x + btn[1][0] + 3, y + 14, ' ');
		}

		// draw the cursor where it should be

		if (cursor_x == 1 || cursor_x == 2) // cursor is at the buttons
		{
			if (cursor_x == 1) // ACCEPT
				cl->outchar(x + btn[0][0] + 3, y + 14, btn_cursor_icon);
			else
				cl->outchar(x + btn[1][0] + 3, y + 14, btn_cursor_icon);
		}
		else if (cursor_x == 0) // cursor is browsing items
		{
			cl->outchar(x+26, y + 4 + cursor_y, itm_cursor_icon_l);
			cl->outchar(x+33, y + 4 + cursor_y, itm_cursor_icon_r);
		}

		int k = cl->get_key();
		
		switch (k)
		{
		case VK_UP:
			{
				// if browsing items
				if (cursor_x == 0)
				{
					if (cursor_y > 0)
					{
						cursor_y--;
					}
					else if (cursor_y <= 0)
					{
						cursor_y = num_items-1;
					}
				}
				else
				{
					// move up to the last item
					// move cursor x
					cursor_y = num_items-1;
					cursor_x = 0;
				}
			} break;
		case VK_DOWN:
			{
				// if browsing items
				if (cursor_x == 0)
				{
					if (cursor_y < num_items-1) 
					{
						cursor_y++;
					}
					else if (cursor_y >= num_items-1)
					{
						// move to the buttons
						cursor_x = 1;
					}
				}
			} break;
		case VK_LEFT:
			{
				if (cursor_x == 0)
				{
					if (item_qty[cursor_y] > 0)
					{
						item_qty[cursor_y]--;
					}
				}
				else
				{
					// cursor is at buttons
					// toggle between them
					if (cursor_x == 1) // Accept is selected
					{
						cursor_x = 2;
					}
					else if (cursor_x == 2) // Cancel is selected
					{
						cursor_x = 1;
					}

				}
			} break;
		case VK_RIGHT:
			{
				if (cursor_x == 0)
				{
					//##
					//## insert patch 11:42PM 3.16.2008
					//## Mage crashes when trying to purchase magic!
					//##

					// if this shop is a magic shop then we only deal with
					// magic spells not items!
					// this was the cause of the runtime crash.
					if (keeper->is_magic_shop)
					{
						// limit magic purchase to one each
						if (!obj_has_spell(player, keeper->spells.at(cursor_y)->name))
						{
							item_qty[cursor_y] = 1; // limit equipment to 1 each
						}
						else
						{
							item_qty[cursor_y] = 0;
							char nobuy[80];
							sprintf_s(nobuy,80,"Cannot purchase more than one %s!",keeper->spells.at(cursor_y)->name);
							cl->show_message(3,"",nobuy,"");
						} // end limit to one
					}
					else
					{
						// this shop has no spells, and is an item shop
						if (keeper->items.at(cursor_y)->can_equip)
						{
							// can we use this item?
							// do not allow purchase if we cannot use it
							if ( (keeper->items.at(cursor_y)->chr_class == player->chr_class) || (keeper->items.at(cursor_y)->chr_class == 9))
							{
								// only allow purchase if we don't have it already!
								if (!obj_has_equipped(player,keeper->items.at(cursor_y)->name))
								{
									item_qty[cursor_y] = 1; // limit equipment to 1 each
								}
								else
								{
									item_qty[cursor_y] = 0;
									char nobuy[80];
									sprintf_s(nobuy,80,"Cannot purchase more than one %s!",keeper->items.at(cursor_y)->name);
									cl->show_message(3,"",nobuy,"");
								}
							}
							else
							{
								// cannot use...cannot purchase
								item_qty[cursor_y] = 0;
								cl->show_message(3,"","Cannot purchase equipment you cannot use!","");
							} // end if cannot use
						}
						else
						{
							if (item_qty[cursor_y] < 255)
							{
								item_qty[cursor_y]++;
							}
						} // end else is not equipment
					} // end else not a magic shop
				}
				else
				{
					// cursor is at buttons
					// toggle between them
					if (cursor_x == 1) // Accept is selected
					{
						cursor_x = 2;
					}
					else if (cursor_x == 2) // Cancel is selected
					{
						cursor_x = 1;
					}
				} // end else is buttons
			} break;

		// CANCEL
		case VK_BACK:
		case VK_ESCAPE:
			{
				// prevent accidentally leaving the shop
				int yesno = cl->show_choice("Exit the Shop?");
				if (yesno == 1)
				{
					form_result = SHOP_OPTION_CANCEL;
					close_box = true;
				}
			} break;

		// ACCEPT
		case VK_SPACE:
		case VK_RETURN:
			{
				if (cursor_x != 0) // cursor is on buttons
				{
					if (cursor_x == 1) // accept
					{
						// calculate the total cost
						unsigned long total = 0;
						{
							for (int n = 0; n < num_items; n++)
							{
								if (item_qty[n] > 0)
								{
									if (keeper->is_magic_shop)
									{
										total += (item_qty[n] * keeper->spells.at(n)->cost);
									}
									else
									{
										total += (item_qty[n] * keeper->items.at(n)->cost);
									}
								}
							}
						}

						if (total > 0) // did we actually make a purchase?
						{
							char totalc[64];
							sprintf_s(totalc, 64, "Your total is $%d.", total);
							// show the cost
							cl->show_message(3, "", totalc, "");
							// prompt user to really purchase
							int yesno = cl->show_choice("Purchase these items?");
							if (yesno == 1)
							{
								// check for enough money
								if (player->gold < total)
								{
									// if not enough money tell user
									cl->show_message(1, "Not enough money!");
								}
								else
								{
									// subtract total
									player->gold -= total;
									// if all ok, then give items to
									int items_gained = 0;
									{
										for(int n = 0; n < num_items; n++)
										{
											if (item_qty[n] > 0)
											{
												for (int q = 0; q < item_qty[n]; q++)
												{
													if (keeper->is_magic_shop)
													{
														player->spells.push_back(get_master_magic(keeper->spells.at(n)->name));
													}
													else
													{
														player->items.push_back(get_master_item(keeper->items.at(n)->name));

														// equip any equipment
														if (keeper->items.at(n)->can_equip)
														{
															if (0!=keeper->items.at(n)->equip_func)
															{
																char m9[40];
																sprintf_s(m9,40,"equipping %s", keeper->items.at(n)->name);
																cl->show_message(4,"Auto-Equip: ON","",m9,"");

																item_target* t = new item_target(
																	keeper->items.at(n), 
																	player);
																keeper->items.at(n)->equip_func(t);
																delete t;
															}
														}
													}
												}
												items_gained += item_qty[n];
											}
										}
									}
									// the user, tell the user
									char gains[64];
									//char bank[64];
									
									draw_shop_ext();

									sprintf_s(gains, 64, "Purchased %d items for $%d.", items_gained, total);
									//sprintf_s(bank, 64, "You have $%d remaining.", player->gold);
									cl->show_message(3, "", gains, "");
									// and then reset the item quantities
									// for the shop so that you can
									// continue to shop.
									{for(int n=0;n<num_items;n++){item_qty[n]=0;}}
								}
							}
							form_result = SHOP_OPTION_ACCEPT;
							close_box = true;
							
						}
						else
						{
							// nothing purchased, tell the user to buy something
							cl->show_message(2,
								"Shop Keeper:",
								"You haven't selected anything to buy!");
						}
					}
					else if (cursor_x == 2) // cancel
					{
						form_result = SHOP_OPTION_CANCEL;
						close_box = true;
					}
				}
				else
				{
					// cursor is at items
					// so show details of the selected item
					// :D a nice feature
					if (!keeper->is_magic_shop)
					{
						display_item_detail(keeper->items.at(cursor_y)->name);
						cl->set_bgcolor(4);
						cl->set_fgcolor(1|2|4|8);
					}
				}
			} break;
		}
	}

	delete [] item_qty;
	
	hide_shop_ext();
	// close the window
	cl->close_window(wnd);

	// cl->close_window(masterwnd);
	return form_result;
}


// the form that is shown when you are selling items
int shop_form_c(void* d)
{
	/*
	char btn_cursor_icon = (char)31;
	char itm_cursor_icon_l = (char)17;
	char itm_cursor_icon_r = (char)16;

	int cursor_x = 0; // cursor begins at the first item
	int cursor_y = 0; // ..

	// the two buttons
	int btn[2][4] =
	{
		{3, 15, 10, 3},
		{21, 15, 10, 3}
	};
	char* btn_caption[2] = {"ACCEPT","CANCEL"};

	// fake the shop data
	char* shop_name = "DUSHAL ITEM SHOP";
	char* item_name[10] =
	{
    //   XXXXXXXXXXXXXXXX    $10000  x255
		"Item Name       ",
		"Item Name       ",
		"Item Name       ",
		"Item Name       ",
		"Item Name       ",
		"Item Name       ",
		"Item Name       ",
		"Item Name       ",
		"Item Name       ",
		"Item Name       "
	};

	int item_cost[10] =
	{
		10000, 10000, 10000, 10000, 10000,
		10000, 10000, 10000, 10000, 10000
	};

	int item_qty[10] = {0,0,0,0,0,0,0,0,0,0}; // 

	int box[] = {40 - (38 / 2), (25 / 2) - (22 / 2), 38, 22};
	int x = box[0] + 2;
	int y = box[1] + 2;
	int caption_x = x - 2 + (box[2] / 2) - strlen(shop_name) / 2;
	char* msg = "What would you like to sell?";
	int msg_x = x - 2 + (box[2] / 2) - strlen(msg) / 2;

	bool close_box = false;

	// draw the form
	clibwindow* wnd = cl->open_window(box[0], box[1], box[2], box[3]);
	
	// draw the caption and the message
	cl->outchars(caption_x, y, shop_name);
	cl->outchars(msg_x, y + 2, msg);

	// draw the available items
	{for(int n = 0; n < 10; n++)
	{
		cl->outchars(x, y + (4 + n), item_name[n]);
		cl->outcharsf(x+20, y + (4 + n), "$%d", item_cost[n]);
	}
	}

	// draw the buttons
	{for(int n = 0; n < 2; n++)
	{
		cl->draw_button(x + btn[n][0], y + btn[n][1], btn[n][2], btn[n][3], btn_caption[n]);
	}
	}

	int form_result = SHOP_OPTION_CANCEL;
	while(!close_box)
	{

		// draw the quantities
		{for(int n = 0; n < 10; n++)
		{
			cl->outchars(x+28, y + (4 + n), "x   "); // clear old
			cl->outcharsf(x+28, y + (4 + n), "x%d", item_qty[n]);
		}
		}

		// clear all cursor positions
		{for(int n=0;n<10;n++){cl->outchar(x+26,y+4+n,' ');cl->outchar(x+33,y+4+n,' ');}
		cl->outchar(x + btn[0][0] + 3, y + 14, ' ');
		cl->outchar(x + btn[1][0] + 3, y + 14, ' ');
		}

		// draw the cursor where it should be

		if (cursor_x == 1 || cursor_x == 2) // cursor is at the buttons
		{
			if (cursor_x == 1) // ACCEPT
				cl->outchar(x + btn[0][0] + 3, y + 14, btn_cursor_icon);
			else
				cl->outchar(x + btn[1][0] + 3, y + 14, btn_cursor_icon);
		}
		else if (cursor_x == 0) // cursor is browsing items
		{
			cl->outchar(x+26, y + 4 + cursor_y, itm_cursor_icon_l);
			cl->outchar(x+33, y + 4 + cursor_y, itm_cursor_icon_r);
		}

		int k = cl->get_key();
		
		switch (k)
		{
		case VK_UP:
			{
				// if browsing items
				if (cursor_x == 0)
				{
					if (cursor_y > 0)
					{
						cursor_y--;
					}
					else if (cursor_y <= 0)
					{
						cursor_y = 9;
					}
				}
				else
				{
					// move up to the last item
					// move cursor x
					cursor_y = 9;
					cursor_x = 0;
				}
			} break;
		case VK_DOWN:
			{
				// if browsing items
				if (cursor_x == 0)
				{
					if (cursor_y < 9) 
					{
						cursor_y++;
					}
					else if (cursor_y >= 9)
					{
						// move to the buttons
						cursor_x = 1;
					}
				}
			} break;
		case VK_LEFT:
			{
				if (cursor_x == 0)
				{
					if (item_qty[cursor_y] > 0)
					{
						item_qty[cursor_y]--;
					}
				}
				else
				{
					// cursor is at buttons
					// toggle between them
					if (cursor_x == 1) // Accept is selected
					{
						cursor_x = 2;
					}
					else if (cursor_x == 2) // Cancel is selected
					{
						cursor_x = 1;
					}

				}
			} break;
		case VK_RIGHT:
			{
				if (cursor_x == 0)
				{
					if (item_qty[cursor_y] < 255)
					{
						item_qty[cursor_y]++;
					}
				}
				else
				{
					// cursor is at buttons
					// toggle between them
					if (cursor_x == 1) // Accept is selected
					{
						cursor_x = 2;
					}
					else if (cursor_x == 2) // Cancel is selected
					{
						cursor_x = 1;
					}
				}
			} break;

		// CANCEL
		case VK_BACK:
		case VK_ESCAPE:
			{
				// prevent accidentally leaving the shop
				int yesno = cl->show_choice("Exit the Shop?");
				if (yesno == 1)
				{
					form_result = SHOP_OPTION_CANCEL;
					close_box = true;
				}
			} break;

		// ACCEPT
		case VK_SPACE:
		case VK_RETURN:
			{
				if (cursor_x != 0) // cursor is on buttons
				{
					if (cursor_x == 1) // accept
					{
						// calculate the total cost
						long total = 0;
						{for(int n=0;n<10;n++){if (item_qty[n]>0)total += (item_qty[n] * item_cost[n]);}}
						char totalc[64];
						sprintf_s(totalc, 64, "Your total is $%d.", total);
						// show the cost
						cl->show_message(3,"",totalc,"");
						// prompt user to really purchase
						int yesno = cl->show_choice("Purchase these items?");
						if (yesno == 1)
						{
							// check for enough money
							if (player->gold < total)
							{
								// if not enough money tell user
								cl->show_message(1, "Not enough money!");
							}
							else
							{
								// subtract total
								player->gold -= total;
								// if all ok, then give items to
								int items_gained = 0;
								{
									for(int n=0;n<10;n++)
									{
										if (item_qty[n] > 0)
										{
											for (int q = 0; q < item_qty[n]; q++)
											{
												player->items.push_back(item_name[n]);
											}
											items_gained += item_qty[n];
										}
									}
								}
								// the user, tell the user
								char gains[64];
								char bank[64];
								sprintf_s(gains, 64, "Purchased %d items for $%d.", items_gained, total);
								sprintf_s(bank, 64, "You have $%d left.", player->gold);
								cl->show_message(3, gains, "", bank);
								// and then reset the item quantities
								// for the shop so that you can
								// continue to shop.
								{for(int n=0;n<10;n++){item_qty[n]=0;}}
							}
						}
						form_result = SHOP_OPTION_ACCEPT;

						{
							int yesno = cl->show_choice("Continue shopping?");
							if (yesno != 1)
							{
								close_box = true;
							}
						}
					}
					else if (cursor_x == 2) // cancel
					{
						form_result = SHOP_OPTION_CANCEL;
						close_box = true;
					}
				}
				else
				{
					// cursor is at items
					// so show details of the selected item
					// :D a nice feature
				}
			} break;
		}
	}

	// close the window
	cl->close_window(wnd);
	return form_result;
	*/
	return 0;
}

