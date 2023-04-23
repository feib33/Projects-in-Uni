/**
 *  This class is to load all coordinates of sprite into an array.
 *  
 *  @author Fei Bao (804522)
 */


import java.io.FileReader;
import java.io.BufferedReader;




public class Loader {	

	private static Sprite[] sprites;
	private static Player player;
	private static final int IS_WALL = 1;
	private static final int NOT_WALL = 0;
	
	public static boolean isBlocked(float x, float y) {
		// Converts a world coordinate to a tile coordinate,
		// and returns if that location is a blocked tile
		for( Sprite sprite : sprites ) {
			if (sprite.tileX == x && sprite.tileY == y 
					&& sprite.flagWall == IS_WALL) {
				return false;
			}
		}
		return true;
	}
		
	/**
	 * Loads the sprites from a given file.
	 * @param filename
	 * @return
	 */
	
	public static Sprite[] loadSprites(String filename){
		// initialize some variables
		int lineNum = 0;
		float mapWidth = 0;
		float mapHeight = 0;
		
		//read file first time and count the number of lines in this file
		try (BufferedReader brFirstly 
				= new BufferedReader(new FileReader(filename))) {
		    lineNum = 0;
		    while ((brFirstly.readLine()) != null) {
		    		lineNum++;
		    }} catch (Exception e1) {
				e1.printStackTrace();
			}
		
		//compute the number of sprites from file 
		//and remove the first line and coordinate of player
		int total = lineNum - 2;
		sprites = new Sprite[total];
		
		//Open file again, read each line and store as String.
		try (BufferedReader brSecondly
				= new BufferedReader(new FileReader(filename))) {
		    String line;
		    total = 0;
		    while ((line = brSecondly.readLine()) != null) {
		    	// when scanning the first line of file,
		    	//store it as width of map and height of map
		    	if (total == 0) {
		    		String[] parts = line.split(",");
		    		float width = Integer.parseInt(parts[0]);
		    		float height = Integer.parseInt(parts[1]);
		    		mapWidth = (App.SCREEN_WIDTH 
		    				- width * App.TILE_SIZE)/2;
		    		mapHeight = (App.SCREEN_HEIGHT 
		    				- height * App.TILE_SIZE)/2;
		    		
		    	} else {
		    		
		    		String[] oneSprite = line.split(",");
		    		// make sure map is at the central of screen
		    		float tileX = 
		    				Float.parseFloat(oneSprite[1])*App.TILE_SIZE + mapWidth;
		    		float tileY = 
		    				Float.parseFloat(oneSprite[2])*App.TILE_SIZE + mapHeight;
		    		int flagWall = NOT_WALL;
		    		
		    		//scan each sprite of file and store it into array of Sprite
		    		String type = oneSprite[0];
		    		switch(type) {
		    		case "floor":
		    			sprites[total-1] = new Sprite("assets/floor.png", 
		    					tileX, tileY, flagWall);
		    			break;
		    		case "stone":
		    			sprites[total-1] = new Sprite("assets/stone.png", 
		    					tileX, tileY, flagWall);
		    			break;
		    		case "target":
		    			sprites[total-1] = new Sprite("assets/target.png", 
		    					tileX, tileY, flagWall);
		    			break;
		    		case "wall":
		    			flagWall = IS_WALL;
		    			sprites[total-1] = new Sprite("assets/wall.png", 
		    					tileX, tileY, flagWall);
		    			flagWall = NOT_WALL;
		    			break;
		    		
		    		//store the coordinate of player in an object
		    		case "player":
		    			player = new Player("assets/player_left.png", 
		    					tileX, tileY);
		    			break;
		    		}
		    		
		    		
		    	}
		    	total++;
		    }
		} catch (Exception e) {
			e.printStackTrace();
		}
		return sprites;
		
	}
	
	public static Player player() {
		//return the object of player
		return player;
	}
}
