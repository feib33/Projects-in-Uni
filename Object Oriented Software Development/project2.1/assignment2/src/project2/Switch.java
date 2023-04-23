package project2;


import org.newdawn.slick.Input;

public class Switch extends Sprite{
	public Switch(float x, float y) {
		super("res/switch.png", x, y);
	}
	
	/**
	 * open the door or close the door
	 */
	@Override
	public void update(Input input, int delta) {
		if(activatedWithBlock()) {
			Door.setClosed(true);
		} else {
			Door.setClosed(false);
		}
	}
}
