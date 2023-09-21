/*开发日志
1.创建项目
2.先导入素材
 3.设计类Block :表示方块
         Trtris:表示游戏相关
4.设计模块主要接口 
*/
#include"Tetris.h"
int main() {
    Tetris game(20, 10, 263, 133, 36);
    game.play();
    return 0;
}