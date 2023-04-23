package project2;

import org.newdawn.slick.Graphics;

public class Door extends Sprite{
	private static boolean closed = false;
	public Door(float x, float y) {
		super("res/door.png", x, y);
	}
	
	/**set the value of door's activated
	 * 
	 * @param bool
	 */
	public static void setClosed(Boolean bool) {
		closed = bool;
	}
	
	/**return the value of whether closing the door
	 * 
	 * @return
	 */
	public static Boolean getClosed() {
		return closed;
	}
	
	/**
	 * render the door or not according to the value of closed
	 */
	@Override
	public void render(Graphics g) {
		for(Sprite sprite : World.getList()) {
			if(sprite instanceof Door & !closed) {
				sprite.getImage().drawCentered(sprite.getX(), sprite.getY());
			}
		}
	}
} 
