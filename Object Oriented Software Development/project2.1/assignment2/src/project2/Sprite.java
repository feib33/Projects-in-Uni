package project2;

import org.newdawn.slick.Image;
import org.newdawn.slick.Input;
import org.newdawn.slick.SlickException;

import java.util.ArrayList;

import org.newdawn.slick.Graphics;

public class Sprite implements Cloneable{
	// Used to decide what direction an object is moving
	// Look up enums to find a more elegant solution!
	public static final int DIR_NONE = 0;
	public static final int DIR_LEFT = 1;
	public static final int DIR_RIGHT = 2;
	public static final int DIR_UP = 3;
	public static final int DIR_DOWN = 4;
	public static final int TILE_SIZE = 32;
	public static final int DOUBLE = 2;
	
	private Image image = null;
	private float x;
	private float y;
	
	public Sprite(String image_src, float x, float y) {
		try {
			image = new Image(image_src);
		} catch (SlickException e) {
			e.printStackTrace();
		}
		
		this.x = x;
		this.y = y;
		//snapToGrid();
	}
	
	
	public void update(Input input, int delta) {
		
	}
	
	/**draw the map at the center of screen
	 * 
	 * @param g
	 */
	public void render(Graphics g) {
		image.drawCentered(x, y);
	}
	
	// Forces this sprite to align to the grid
	public void snapToGrid() {
		x /= App.TILE_SIZE;
		y /= App.TILE_SIZE;
		x = Math.round(x);
		y = Math.round(y);
		x *= App.TILE_SIZE;
		y *= App.TILE_SIZE;
	}
	
	/**check the class of object after player where will move to
	 * 
	 * @param dir
	 */
	public void moveToDest(int dir) {
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
		
		// when cracked wall is under the tnt and tnt is under the player, then destroy both
		if(getNameOfSprite(x+delta_x, y+delta_y) == "project2.Tnt"
				&& getNameOfSprite(x + DOUBLE*delta_x, y + DOUBLE*delta_y) == "project2.Cracked") {
			
			Tnt.onMove(x, y,x+delta_x, y+delta_y);
		}
		// Make sure the position isn't occupied!
		if(isPushable(x + delta_x, y + delta_y)) {
			if(!isPushable(x +DOUBLE*delta_x, y + DOUBLE*delta_y) 
					&& !isBlocked(x +DOUBLE*delta_x, y + DOUBLE*delta_y)) {
				if(getSprite(x+delta_x, y+delta_y) instanceof Ice) {
					Ice.setActivated(true);
					Ice.setDir(dir);
				}
				
				x += delta_x;
				y += delta_y;
				
				Player.onMove(x, y, x + delta_x, y + delta_y);
				
				
				
			}
		}else if(!isBlocked(x + delta_x, y + delta_y)){
			// if player moved, then increase the moves
			if((int)delta_x == 0 && (int)delta_y == 0) {
				//Player did not move at this moment.
			} 
			// when touch the units, restart the current level
			if(getSprite(x+delta_x, y+delta_y) instanceof Skeleton
					|| getSprite(x+delta_x, y+delta_y) instanceof Mage
					|| getSprite(x+delta_x, y+delta_y) instanceof Rogue) {
				World.restart();
				Player.setMoves(0);
			}
			x += delta_x;
			y += delta_y;
		}
	}
	
	/**return the name of class where coordinate is (x, y)
	 * 
	 * @param x  coordinate of x
	 * @param y  coordinate of y
	 * @return
	 */
	public String getNameOfSprite(float x, float y) {
		for(Sprite sprite : World.getList()) {
			if(sprite.getX() == x
					&& sprite.getY() == y
					&& sprite.getClass().getName() != "project2.Floor"
					) {
				return sprite.getClass().getName();
			}
		}
		return null;
	}
	
	/**get the sprite of the coordinate (x, y)
	 * 
	 * @param x
	 * @param y
	 * @return
	 */
	public static Sprite getSprite(float x, float y) {
		for(Sprite sprite : World.getList()) {
			if(sprite.getX() == x 
					&& sprite.getY() == y
					&& !(sprite instanceof Floor)) {
				return sprite;
			}
		}
		return null;
	}
	
	/**check the coordinate (x, y) is occupied or not
	 * 
	 * @param x
	 * @param y
	 * @return
	 */
	public static Boolean isBlocked(float x, float y) {
		if(getSprite(x, y) instanceof Wall
				|| getSprite(x, y) instanceof Cracked
				|| (getSprite(x, y) instanceof Door && !Door.getClosed())) {
			return true;
		}
		return false;
	}
	
	/** check the coordinate (x, y) is pushable or not
	 * 
	 * @param x
	 * @param y
	 * @return
	 */
	public static Boolean isPushable(float x, float y) {
		for(Sprite sprite : World.getList()) {
			if(sprite.getX() == x
					&& sprite.getY() == y
					&& (sprite instanceof Ice
							|| sprite instanceof Stone
							|| sprite instanceof Tnt)) {
				return true;
			} 			
		}
		return false;
	}
	
	/** check this sprite is activated or not
	 * 
	 * @return
	 */
	public Boolean activatedWithBlock() {
		for(Sprite sprite : World.getList()) {
			if(sprite.getX() == x && sprite.getY() == y && isPushable(x, y)) {
				return true;
			}
		}
		return false;
	}
	
	
	public Object clone() throws CloneNotSupportedException{  
		return super.clone();  
	}
	
	/**get the image of current sprite
	 * 
	 * @return
	 */
	public Image getImage() {
		return image;
	}
	
	/**get the coordinate of x of current sprite
	 * 
	 * @return
	 */
	public float getX() {
		return x;
	}
	
	/**set the coordinate of x of current sprite
	 * 
	 * @param x
	 */
	public void setX(float x) {
		this.x = x;
	}
	
	/**get the coordinate of y of current sprite
	 * 
	 * @return
	 */
	public float getY() {
		return y;
	}
	
	/**set coordinate of y of current sprite
	 * 
	 * @param y
	 */
	public void setY(float y) {
		this.y = y;
	}
}
