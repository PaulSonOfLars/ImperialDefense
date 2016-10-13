@ this file should contain all the assets used

.section .data
.globl bin_img_loading
.align 4
bin_img_loading:
  .incbin "images/loading.img"

.globl bin_img_cursor
.align 4
bin_img_cursor:
  .incbin "images/mouse.img"

.globl bin_img_decoy
.align 4
bin_img_decoy:
  .incbin "images/credits.img"

.globl bin_img_mo_decoys
.align 4
bin_img_mo_decoys:
  .incbin "images/credits.img"

.globl bin_img_credits
.align 4
bin_img_credits:
  .incbin "images/credits.img"
@--------------GAME LAYOUT----------------

.globl bin_img_view_select_default
.align 4
bin_img_view_select_default:
  .incbin "images/game_layout/view_select_default.img"

.globl bin_img_view_select_tower
.align 4
bin_img_view_select_tower:
  .incbin "images/game_layout/view_select_tower.img"

.globl bin_img_view_select_upgrade
.align 4
bin_img_view_select_upgrade:
  .incbin "images/game_layout/view_select_upgrade.img"

.globl bin_img_pretty_play
.align 4
bin_img_pretty_play:
  .incbin "images/game_layout/highlight\ buttons/play_menu.img"

.global bin_img_pretty_exit
.align 4
bin_img_pretty_exit:
  .incbin "images/game_layout/highlight\ buttons/exit_menu.img"

.globl bin_img_pretty_menu
.align 4
bin_img_pretty_menu:
  .incbin "images/game_layout/highlight\ buttons/menu_ingame.img"

.globl bin_img_pretty_archer
.align 4
bin_img_pretty_archer:
  .incbin "images/game_layout/highlight\ buttons/archer_ingame.img"

.globl bin_img_pretty_canon
.align 4
bin_img_pretty_canon:
  .incbin "images/game_layout/highlight\ buttons/canon_ingame.img"

.globl bin_img_pretty_mage
.align 4
bin_img_pretty_mage:
  .incbin "images/game_layout/highlight\ buttons/mage_ingame.img"

.globl bin_img_pretty_speed
.align 4
bin_img_pretty_speed:
  .incbin "images/game_layout/highlight\ buttons/speed_ingame.img"

.globl bin_img_pretty_range
.align 4
bin_img_pretty_range:
  .incbin "images/game_layout/highlight\ buttons/range_ingame.img"

.globl bin_img_pretty_dmg
.align 4
bin_img_pretty_dmg:
  .incbin "images/game_layout/highlight\ buttons/dmg_ingame.img"

.globl bin_img_pretty_mov
.align 4
bin_img_pretty_mov:
  .incbin "images/game_layout/highlight\ buttons/mov_ingame.img"

.globl bin_img_pretty_del
.align 4
bin_img_pretty_del:
  .incbin "images/game_layout/highlight\ buttons/del_ingame.img"

.globl bin_img_pretty_cancel
.align 4
bin_img_pretty_cancel:
  .incbin "images/game_layout/highlight\ buttons/cancel_ingame.img"


@-------------GAME TILES-----------------

.globl bin_img_more_decoys
.align 4
bin_img_more_decoys:
  .incbin "images/level1.img"

.globl bin_img_map_one
.align 4
bin_img_map_one:
  .incbin "images/level1.img"

.globl bin_img_map_two
.align 4
bin_img_map_two:
  .incbin "images/level2.img"

.globl bin_img_map_three
.align 4
bin_img_map_three:
  .incbin "images/level3.img"

@.globl bin_img_cornerDL
@bin_img_cornerDL:
@  .incbin "images/tiles/cornerDownLeft.img"
@
@.globl bin_img_cornerUL
@bin_img_cornerUL:
@  .incbin "images/tiles/cornerUpLeft.img"
@
@.globl bin_img_cornerDR
@bin_img_cornerDR:
@  .incbin "images/tiles/cornerDownRight.img"
@
@.globl bin_img_cornerUR
@bin_img_cornerUR:
@  .incbin "images/tiles/cornerUpRight.img"
@
@.globl bin_img_pathHor
@bin_img_pathHor:
@  .incbin "images/tiles/pathHor.img"
@
@.globl bin_img_pathVer
@bin_img_pathVer:
@  .incbin "images/tiles/pathVer.img"
@
@.globl bin_img_grass
@bin_img_grass:
@  .incbin "images/tiles/grass.img"
@
@.globl bin_img_grassflowers
@bin_img_grassflowers:
@  .incbin "images/tiles/grassflowers.img"
@
@.globl bin_img_grasswood
@bin_img_grasswood:
@  .incbin "images/tiles/grasswood.img"
@
@.globl bin_img_bricks
@bin_img_bricks:
@  .incbin "images/tiles/bricks.img"

@------------ENEMIES----------------

.globl bin_img_barbar
.align 4
bin_img_barbar:
  .incbin "images/enemies/barbar.img"

.globl bin_img_boar
.align 4
bin_img_boar:
  .incbin "images/enemies/boar.img"

@-----------TOWERS---------------

.globl bin_img_archer
.align 4
bin_img_archer:
.incbin "images/towers/archer.img"

.globl bin_img_cannon
.align 4
bin_img_cannon:
  .incbin "images/towers/cannon.img"

.globl bin_img_mage
.align 4
bin_img_mage:
  .incbin "images/towers/mage.img"

.globl bin_img_font
.align 4
bin_img_font:
  .incbin "images/font.img"

.globl bin_img_projArrow
.align 4
bin_img_projArrow:
  .incbin "images/projectiles/arrow.img"

.globl bin_img_projCannon
.align 4
bin_img_projCannon:
  .incbin "images/projectiles/cannonball.img"

.globl bin_img_projMagic
.align 4
bin_img_projMagic:
  .incbin "images/projectiles/magicball.img"

.globl bin_img_blood
.align 4
bin_img_blood:
  .incbin "images/enemies/blood.img"


.globl bin_snd_bgmusic
.align 4
bin_snd_bgmusic:
  .incbin "sounds/bgmusic.bin"
bin_snd_bgmusic_eof:

.globl bin_img_archer_hover
.align 4
bin_img_archer_hover:
  .incbin "images/tower_info_hovers/archer.img"

.globl bin_img_canon_hover
.align 4
bin_img_canon_hover:
  .incbin "images/tower_info_hovers/canon.img"

.globl bin_img_mage_hover
.align 4
bin_img_mage_hover:
  .incbin "images/tower_info_hovers/mage.img"

.globl bin_img_levelpick
.align 4
bin_img_levelpick:
  .incbin "images/levelpick.img"

.globl bin_img_level_1h
.align 4
bin_img_level_1h:
  .incbin "images/level higlight buttons/1h.img"

.globl bin_img_level_2h
.align 4
bin_img_level_2h:
  .incbin "images/level higlight buttons/2h.img"

.globl bin_img_level_3h
.align 4
bin_img_level_3h:
  .incbin "images/level higlight buttons/3h.img"

.globl bin_img_back_h
.align 4
bin_img_back_h:
  .incbin "images/level higlight buttons/backh.img"

.globl bin_img_tile_sel
.align 4
bin_img_tile_sel:
  .incbin "images/tiles/tileSelected.img"

.align 2
