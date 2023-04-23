package project2;

import java.util.ArrayList;

import org.newdawn.slick.Input;

public class Player extends Sprite {
	private static ArrayList<Sprite> sprites = new ArrayList<Sprite>();
	private static int moves = 0;
	public Player(float x, float y) {
		super("res/player_left.png", x, y);
	}
	
	/**
	 * update the coordinate of the player
	 */
	@Override
	public void update(Input input, int delta){
		int dir = DIR_NONE;

		if (input.isKeyPressed(Input.KEY_LEFT)) {
			dir = DIR_LEFT;
			moves++;
			World.addIntoStack();
			Rogue.onMove(-TILE_SIZE);
			Mage.setActivated(true);
		}
		else if (input.isKeyPressed(Input.KEY_RIGHT)) {
			dir = DIR_RIGHT;
			moves++;
			World.addIntoStack();
			Rogue.onMove(-TILE_SIZE);
			Mage.setActivated(true);
		}
		else if (input.isKeyPressed(Input.KEY_UP)) {
			dir = DIR_UP;
			moves++;
			World.addIntoStack();
			Rogue.onMove(-TILE_SIZE);
			Mage.setActivated(true);
		}
		else if (input.isKeyPressed(Input.KEY_DOWN)) {
			dir = DIR_DOWN;
			moves++;
			World.addIntoStack();
			Rogue.onMove(-TILE_SIZE);
			Mage.setActivated(true);
		}
		
		
		// Move to our destination
		moveToDest(dir);
		
	}  
	
	
	/** Move the stone one step
	 * 
	 * @param testX   the x of original stone
	 * @param testY	  the y of original stone
	 * @param desX	the x of moved stone
	 * @param desY	the y of moved stone
	 */
	public static void onMove(float testX, float testY, float desX, float desY) {
		sprites = World.getList();
		for(Sprite sprite : sprites) {
			if (sprite.getX() == testX 
					&& sprite.getY() == testY 
					&& (sprite instanceof Stone
					|| sprite instanceof Ice 
					|| sprite instanceof Tnt)) {
				sprite.setX(desX);
				sprite.setY(desY);
			}
		}
	}
	
	
	/**get the moves
	 * 
	 * @return
	 */
	public static int getMoves() {
		return moves;
	}
	
	/**set the moves
	 * 
	 * @param moves the number of move-times
	 */
	public static void setMoves(int moves) {
		Player.moves = moves;
	}
}
