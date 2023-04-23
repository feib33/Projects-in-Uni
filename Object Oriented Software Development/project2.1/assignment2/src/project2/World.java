package project2;

import java.util.ArrayList;
import org.newdawn.slick.Graphics;
import org.newdawn.slick.Input;

public class World {
	private static ArrayList<Sprite> sprites;
	private static int level = 1;
	int moves = 0;
	public World() {
		sprites = Loader.loadSprites("res/levels/" + level + ".lvl");
		
	}
	
	/**update the each map
	 * 
	 * @param input
	 * @param delta
	 */
	public void update(Input input, int delta) {
		for (Sprite sprite : sprites) {
			if (sprite != null) {
				sprite.update(input, delta);
				undo(input, delta);
				nextLevel();
				
			}
		}
	}
	
	public void render(Graphics g) {
		for (Sprite sprite : sprites) {
			if (sprite != null) {
				sprite.render(g);
			}
		}
		moves = Player.getMoves();
		g.drawString("Moves: " + moves, 0, 0);
	}
	
	
	/**undo the last action 
	 * 
	 * @param input
	 * @param delta
	 */
	public void undo(Input input, int delta) {
		if (input.isKeyPressed(Input.KEY_Z)) {
			moves--;
			if( !HistoryStack.empty()) {
				sprites = HistoryStack.pop();
				Player.setMoves(moves);
			}
		}  else if (input.isKeyPressed(Input.KEY_R)) {
			sprites = Loader.loadSprites("res/levels/" + level + ".lvl");
			Player.setMoves(0);
		} 
	}
	
	/**
	 * restart the current level
	 */
	public static void restart () {
		sprites = Loader.loadSprites("res/levels/" + level + ".lvl");
	}
	
	/**
	 * add the map into stack
	 */
	public static void addIntoStack() {
		try {
			HistoryStack.push(sprites);
		} catch (CloneNotSupportedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	/**return the map
	 * 
	 * @return
	 */
	public static ArrayList<Sprite> getList() {
		return sprites;
	} 
	
	/**
	 * when all targets are activated, then load the next level
	 */
	public void nextLevel () {
		int numTarget = 0, numActivated = 0;
		for(Sprite sprite : sprites) {
			if(sprite instanceof Target) {
				numTarget++;
				if(Target.isActivated(sprites, sprite)) {
					numActivated++;
				}
			}
		}
		
		// when all targets are activated, load next level
		if(numActivated == numTarget) {
			level++;
			sprites = Loader.loadSprites("res/levels/" + level + ".lvl");
			Player.setMoves(0);
		}
	}
}
