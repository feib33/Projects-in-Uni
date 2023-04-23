package project2;

import org.newdawn.slick.Input;

public class Skeleton extends Sprite{
	public static final int SOUND_MILLION_SECOND = 1000;
	private int time = 0;
	private static int dir = DIR_UP;
	public Skeleton(float x, float y) {
		super("res/skull.png", x, y);
	}
	
	@Override
	public void update(Input input, int delta) {
		if(time >= SOUND_MILLION_SECOND) {
			
			onMove(dir);
			time = 0;
		} else {
			time += delta;
		}
		

	}
	
	/**move the skeleton
	 * 
	 * @param dir
	 */
	public void onMove(int dir) {
		float speed = TILE_SIZE;
		// Translate the direction to an x and y displacement
		float delta_y = 0;
		switch (dir) {
			case DIR_UP:
				delta_y = -speed;
				break;
			case DIR_DOWN:
				delta_y = speed;
				break;
		}
		
		for(Sprite sprite : World.getList()) {
			if(sprite instanceof Skeleton) {
				if(dir == DIR_UP) {
					// check skeleton will not move when touch wall or others
					if(!isBlocked(sprite.getX(), sprite.getY() + delta_y)
							&& !isPushable(sprite.getX(), sprite.getY() + delta_y)) {
						sprite.setY(sprite.getY()+delta_y);
						
					}else {
						setDir(DIR_DOWN);
					}
				} else if(dir == DIR_DOWN) {
					if(!isBlocked(sprite.getX(), sprite.getY() + delta_y)
							&& !isPushable(sprite.getX(), sprite.getY() + delta_y)){
						sprite.setY(sprite.getY()+delta_y);
					}else {
						setDir(DIR_UP);
					}
				}
			}
		}
	}
	
	/**set the value of direction
	 * 
	 * @param direction
	 */
	public static void setDir(int direction) {
		Skeleton.dir = direction;
	}
}
