/** 
 * This class is the constructor of Sprite and draw the map 
 * from the parsed data.
 * 
 * @author Fei Bao (804522)
 */

import org.newdawn.slick.Input;
import org.newdawn.slick.SlickException;
import org.newdawn.slick.Image;
import org.newdawn.slick.Graphics;

public class Sprite {

	public Image tileType;
	public float tileX;
	public float tileY;
	public int flagWall;
	public Sprite(String image_src, 
			float x, float y, int flag) throws SlickException {
		tileType = new Image(image_src);
		tileX = x;
		tileY = y;
		flagWall = flag;
	}
	
	public static void update(Input input, int delta) {
	//for project 2 in the future. maybe the movement of stone?
 	}
	
	public static void render(Graphics g, Sprite[] sprites) {
		//draw each tile in the array of sprite
		for(Sprite sprite : sprites) {
			g.drawImage(sprite.tileType, sprite.tileX, sprite.tileY);
		}
	}
}
