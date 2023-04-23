package project2;



public class Rogue extends Sprite{
	private static int dir = DIR_LEFT;
	public Rogue(float x, float y) {
		super("res/rogue.png", x, y);
	}
	
	/**move the rogue
	 * 
	 * @param delta_x
	 */
	public static void onMove(float delta_x) {
		for(Sprite sprite : World.getList()) {
			if(sprite instanceof Rogue) {
				if(dir == DIR_LEFT) {
					// check the rogue will not move when touch wall
					if(!isBlocked(sprite.getX()+delta_x, sprite.getY())){
						sprite.setX(sprite.getX()+delta_x);
					}else {
						//reverse the direction when touch the wall
						dir = DIR_RIGHT;
					}
				} else if(dir == DIR_RIGHT) {
					// check the rogue will not move when touch wall
					if(!isBlocked(sprite.getX()-delta_x, sprite.getY())){
						sprite.setX(sprite.getX()-delta_x);
					}else {
						//reverse the direction when touch the wall
						dir = DIR_LEFT;
					}
				}
			}
		}
	}
	
	
}
