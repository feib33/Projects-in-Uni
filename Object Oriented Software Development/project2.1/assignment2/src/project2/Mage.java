package project2;

import org.newdawn.slick.Input;

public class Mage extends Sprite{
	
	private static int dir = 0;
	private static boolean activated = false;
	public Mage(float x, float y) {
		super("res/mage.png", x, y);
	}
	
	/**move the mage
	 * 
	 * @param x
	 * @param y
	 */
	public void onMove(float x, float y) {
		for(Sprite sprite : World.getList()) {
			if(sprite instanceof Player) {
				System.out.println("the coor of player is " + x + "  " + y);
				float playerX = sprite.getX();
				float playerY = sprite.getY();
				float distX = x - playerX;
				float distY = y - playerY;
				System.out.println("the coor is " + distX + "  " + distY);
				if(distX < 0) {
					dir = -1;
				}else {
					dir = 1;
				}
				if(Math.abs(distX) > Math.abs(distY) 
						&& !isBlocked(x + TILE_SIZE * dir, x)
						&& !isPushable(x + TILE_SIZE * dir, y)) {
						moveToDes(x+TILE_SIZE*dir, y);
				}else {
					if(!isBlocked(x, y + TILE_SIZE * dir)
							&& !isPushable(x, y + TILE_SIZE * dir)){
						moveToDes(x, y + TILE_SIZE * dir);
					}
				}
			}
		}
	}
	
	@Override
	public void update(Input input, int delta) {
		for(Sprite sprite : World.getList()) {
			if(sprite instanceof Mage & activated) {
				onMove(sprite.getX(), sprite.getY());
				setActivated(false);
				break;
			}
		}
	}
	
	public void moveToDes(float x, float y) {
		for(Sprite sprite : World.getList()) {
			if(sprite instanceof Mage) {
				sprite.setX(x);
				sprite.setY(y);
			}
		}
	}
	
	public static void setActivated(boolean bool) {
		activated = bool;
	}
}
