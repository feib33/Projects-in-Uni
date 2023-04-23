package project2;

import java.util.ArrayList;

public class Tnt extends Sprite{
	private static ArrayList<Sprite> sprites = new ArrayList<Sprite>();
	public Tnt(float x, float y) {
		super("res/tnt.png", x, y);
	}
	
	public static void onMove(float x, float y, float delta_x, float delta_y) {
		sprites = World.getList();
		int i = 0;
		for(Sprite sprite : sprites) {
			i++;
			if(sprite instanceof Tnt) {
				sprites.set(i-1, Loader.createSprite("floor", x + delta_x, y + delta_y));
			}
			if (sprite instanceof Cracked) {	
				sprites.set(i-1, Loader.createSprite("floor", x + 2*delta_x, y + 2*delta_y)); 
			}
		}
	}
}
