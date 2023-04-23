package project2;

import java.util.ArrayList;

public class Target extends Sprite {
	private static boolean activated = false;
	
	public Target(float x, float y) {
		super("res/Target.png", x, y);
	}
	
	
	/**check the current target is activated or not
	 * 
	 * @param sprites 	the arraylist of sprites
	 * @param target	the sprite of target
	 * @return
	 */
	public static boolean isActivated(ArrayList<Sprite> sprites, Sprite target) {
		for(Sprite sprite : sprites) {
			if(sprite.getX() == target.getX()
					&& sprite.getY() == target.getY()
					&& sprite.activatedWithBlock()) {
				activated = true;
				return activated;
			}
		}
		return false;
	}
	
	
	
	
}
