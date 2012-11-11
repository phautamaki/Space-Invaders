Row 1 consists of TILE_TYPE_BLOCKING_BREAKABLE except the first tile which is empty
- This means that these tiles will be marked into .map files as
1:2
2:2
3:2

Row 2 consists of empty TILE_TYPE_NONE tiles which will not collide with entities
- These will be marked into .map files as
4:0
5:0
6:0
7:0


Rows 3 and 4 consist of TILE_TYPE_BLOCKING 
- These will be marked into .map files as
8:1
9:1
10:1
...
15:1