# 碰撞检测
>终于到了碰撞检测(Collision Detection)了，还有什么比这更"游戏"的呢？ 游戏中的物体交互大多通过它们来完成的(除非是gal game..)。

碰撞检测主要分为两种:
1.  基于边框的碰撞检测(Bounding Box Collision)
2.  基本距离的碰撞检测(Radial Collision)

## 1. 基于边框的碰撞检测
我们以sprite的x,y以及width，height绘制一个rect作为边框，只有只用检测两个sprite之间是否有交集就(intersection)行了, 需要注意的是sprite是可以缩放的，因此我们创建的边框应该要能和sprite的缩放同步，其实也就是将边框的边长乘上缩放因子就好了。
```
#define SET_RECT(sprite) {(long)sprite.x,(long)sprite.y, (long)sprite.x + sprite.width * sprite.scaling, (long)sprite.y + sprite.height * sprite.scaling}
//bounding box collision detection
bool Collision(SPRITE sprite1, SPRITE sprite2) {
	RECT rect1 = SET_RECT(sprite1);
	RECT rect2 = SET_RECT(sprite2);
	RECT dest; //ignored
	return IntersectRect(&dest, &rect1, &rect2);
}
```
## 2. 基本距离的碰撞检测
如果...sprite不那么方,或者说sprite比较圆的情况下，基于边框的碰撞检测就不显得那么真实了，影响玩家心情或者..心智。
事实上，这就是去比较两个sprite的中点距离和两个sprite的半径和的关系，从而判断两个sprite是否有交集。
```
#define CALC_RADIUS(sprite,radius,center_point)\
	do{\
		if(sprite.width > sprite.height)\
			radius = sprite.width * sprite.scaling / 2.0;\
		else\
			radius = sprite.height * sprite.scaling / 2.0;\
		center_point.x = sprite.x + sprite.width / 2.0; \
		center_point.y = sprite.y + sprite.height / 2.0;\
	}while(0)	
//distance collision detection
bool CollisionD(SPRITE sprite1, SPRITE sprite2) {
	double radius1;
	D3DXVECTOR2 vector1;
	//calculate radius1 ,vector1
	CALC_RADIUS(sprite1, radius1,vector1);

	//calculate radius1 ,vector1
	double radius2;
	D3DXVECTOR2 vector2;
	CALC_RADIUS(sprite2, radius2,vector2);

	double dx = vector1.x - vector2.x;
	double dy = vector1.y - vector2.y;
	double dis = sqrt(dx*dx + dy * dy);
	return dis < radius1 + radius2;
}
```
