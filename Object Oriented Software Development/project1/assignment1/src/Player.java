/**
 *  This class is the constructor of player and,
 *   draw player and check the location of player 
 *   
 *   @author Fei Bao (804522)
 */


import org.newdawn.slick.Graphics;
import org.newdawn.slick.Image;
import org.newdawn.slick.Input;
import org.newdawn.slick.SlickException;

public class Player {

	Player player;
	private Image playerType;
	private static float playerX;
	private static float playerY;
	public Player(String image_src, float x, float y) throws SlickException {
		playerType = new Image(image_src);
		playerX = x;
		playerY = y;
	}
	
	public void update(Input input, int delta) {
		//check whether the next coordinate player will walk to is wall;
		if (input.isKeyPressed(Input.KEY_UP) 
				&& Loader.isBlocked(playerX, playerY-App.TILE_SIZE)) {
			playerY -= App.TILE_SIZE;
		}
		if (input.isKeyPressed(Input.KEY_DOWN) 
				&& Loader.isBlocked(playerX, playerY+App.TILE_SIZE)) {
			playerY += App.TILE_SIZE;
		}
		if (input.isKeyPressed(Input.KEY_LEFT) 
				&& Loader.isBlocked(playerX-App.TILE_SIZE, playerY)) {
			playerX -= App.TILE_SIZE;
		}
		if (input.isKeyPressed(Input.KEY_RIGHT) 
				&& Loader.isBlocked(playerX+App.TILE_SIZE, playerY)) {
			playerX += App.TILE_SIZE;
		}
 	}
	
	public static void render(Graphics g, Player player) {
		//draw the tile of player
		g.drawImage(player.playerType, playerX, playerY);
	}
}
