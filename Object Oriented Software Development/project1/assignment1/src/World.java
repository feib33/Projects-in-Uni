/**
 * This class is to combine some methods from other class
 * 
 * @author Fei Bao (804522)
 */

import org.newdawn.slick.Graphics;
import org.newdawn.slick.Input;
import org.newdawn.slick.SlickException;

public class World {
	
	private Sprite[] sprites;
	private Player player;
	public World() throws  SlickException  
	{
		sprites = Loader.loadSprites("assets/levels/0.lvl");
		player = Loader.player();
	}
	
	public void update(Input input, int delta) 
	{
		player.update(input, delta);
	}
	
	public void render(Graphics g) {
		Sprite.render(g, sprites);
		Player.render(g, player);
	}
}
