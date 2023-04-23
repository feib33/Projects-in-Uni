package project2;

import org.newdawn.slick.Input;

public class Ice extends Sprite{
	private static int dir = 0;
	private static boolean activated = false;
	private float time = 0;
	public static final int QUATOR_SECOND = 250;
	public Ice(float x, float y) {
		super("res/ice.png", x, y);
	}
	
	/**
	 * update the coordinates of ice
	 */
	@Override
	public void update(Input input, int delta) {
		// update per second
		if(time >= QUATOR_SECOND) {
			if(activated) {
				onMove(dir);
			}
			time = 0;
		} else {
			time += delta;
		}
		

	}
	
	/**move the ice
	 * 
	 * @param dir
	 */
	public void onMove(int dir) {
		float speed = TILE_SIZE;
		// Translate the direction to an x and y displacement
		float delta_x = 0,
			  delta_y = 0;
		switch (dir) {
			case DIR_LEFT:
				delta_x = -speed;
				break;
			case DIR_RIGHT:
				delta_x = speed;
				break;
			case DIR_UP:
				delta_y = -speed;
				break;
			case DIR_DOWN:
				delta_y = speed;
				break;
		}
		
		for(Sprite sprite : World.getList()) {
			if(sprite instanceof Ice) {
				if(!Sprite.isBlocked(sprite.getX() + delta_x, sprite.getY() + delta_y)
						&& !Sprite.isPushable(sprite.getX() + delta_x, sprite.getY() + delta_y)) {
					sprite.setX(sprite.getX() + delta_x);
					sprite.setY(sprite.getY() + delta_y);
				}else {
					setActivated(false);
				}
			}
		}
	}
	
	/**set the value of activated
	 * 
	 * @param bool
	 */
	public static void setActivated(boolean bool) {
		Ice.activated = bool;
	}
	
	/**set the value of direction(dir)
	 * 
	 * @param direction
	 */
	public static void setDir(int direction) {
		Ice.dir = direction;
	}
}
