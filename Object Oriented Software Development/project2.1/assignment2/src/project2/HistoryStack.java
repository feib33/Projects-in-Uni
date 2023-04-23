package project2;

import java.util.ArrayList;
import java.util.Stack;

public class HistoryStack {
	private static Stack<ArrayList<Sprite>> history = new Stack<ArrayList<Sprite>>(); 
	
	/**save different time of maps and save them into the stack
	 * 
	 * @param sprites
	 * @throws CloneNotSupportedException
	 */
	public static void push(ArrayList<Sprite> sprites) throws CloneNotSupportedException {
		ArrayList<Sprite> temp = new ArrayList<Sprite>();
		for(Sprite sprite : sprites) {
			temp.add((Sprite)sprite.clone());
		}
		history.push(temp);
		
		
	}
	
	/**return the last stack 
	 * 
	 * @return
	 */
	public static ArrayList<Sprite> pop() {
		return history.pop();
		
	}
	
	/**clear the stack
	 * 
	 * @return
	 */
	public static boolean empty() {
		return history.empty();
	}
	
}
