========================================================================
 EX5 - Save the King (Save princess liea)
========================================================================

Coded by: Darina Poyarkov - 316775402

========================================================================

The game is based on the game Save the king, with a character changes:
The king -  princess liea.
The dwarfs - The white troopers.
The Mage - Yoda.
The Giant - Chuwbacca.
The Thief - Han Solo.
The Thron - space ship.

To change character - press on it with the mouse
========================================================================
To add more levels you need to make the same file with the name "levelX.stk"
- The game can support until 999 levels - withought any changes
========================================================================

Design:

The desing is based on the collision handler- the difference between all the character , is how they act when they
stepping on some object.
Everyone can walk on BLANK , but only the king can get on the thron, only the thief can take the key and then open
the gate, only giant can get on the dwarf and only the mage can get on the fire.
According to that : everyone is entitiy , but with different collision handler.

The abstract class is the entity placer which the board inherits, and the entity knows.
The entity checks if he can remove it, but the board is the one who removes it from it.

Each entity has it's own timer - for a smooth walking. - ENTITY_TICK.

All the entities have the same movings functions.

The menu has it's own class, which holds buttons- buttons hold all the buttons (button is a class too).
About has it's own class, it takes care of itslef.

========================================================================

Made by me files:

-About.h , About.cpp- Takes care about all the about window and text moving

-Board.h , Board.cpp- Takes care about all the printing the sprites and movings of the entities. 

-Button.h , Button.cpp- Takes care about all the buttons on the menu, and it's pressing.

-Buttons.h, Buttons.cpp- Holds all the buttons in the array.

-CollisionHandler.h - Takes care about all the default collison of the entities (blank) and sends it to the needed
		      collision of the entity.

-CollisioHandleFactory.h- It sends the needed collision to the entity - by the simbol.

-Entity.h , Entity.cpp- Each entity of the board is represented as an entity (even static one).

-EntityPlacer.h - Abstruct class- Will remove the entity in the (x,y) coordinates when the character is getting on it

-EntityTimer.h, EntityTimer.cpp - Each entity has it's timer for "smooth" moving.

-GiantCollisionHandler.h- Takes care of the Giant collision rules (Can get on the Dwarf and Blank).

-KingCollisionHandler.h- Takes care of the King collision rules (Can get on the Throne and Blank).

-MagetCollisionHandler.h- Takes care of the Mage collision rules (Can get on the Fire and Blank).

-ThiefCollisionHandler.h- Takes care of the Thief collision rules (Can get on the Key, Gate (with a key) and Blank).

-Menu.h , Menu.cpp- Holds the buttons , represent all the menu window.

-MenuActions.h- Holds the actions of the menu buttons.

-ex5.cpp- Has all the game loop in it (kind of controller).

========================================================================
Algorythems:

The smooth moving of the characters.

The algorythem for making game limits (by seconds or moves)

Using c for loading the level files
========================================================================
Known bugs:

- No different colors for the wall

========================================================================


