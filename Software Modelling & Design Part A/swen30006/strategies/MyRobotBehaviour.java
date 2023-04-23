/**
 * SWEN30006 Project A
 * @author Fei BAO
 * @studentID 804522
 */
package strategies;

import automail.PriorityMailItem;
import automail.StorageTube;

public class MyRobotBehaviour implements IRobotBehaviour{
	
	private boolean newPriority;
	private boolean strong;
	
	public MyRobotBehaviour(boolean strong){
		newPriority = false;
		this.strong = strong;
	}

	@Override
	public void startDelivery() {
		newPriority = false;
	}

	@Override
	public boolean returnToMailRoom(StorageTube tube) {
		if (tube.isEmpty()) {
			return true; // Empty tube means we are returning anyway
		} else {
			// Return if we don't have a priority item 
			//and no more than one normal mail in the tube 
			//and a new one came in
			Boolean priority = (tube.peek() instanceof PriorityMailItem ) ;
			Boolean remainMail = (tube.getSize() < 2);
			return !priority && remainMail && newPriority;
		}
	}

	@Override
	public void priorityArrival(int priority, int weight) {
		if(weight <= 2000 || strong == true) {
			newPriority = true; 
		}else {
			newPriority = false; // Don't return if this robot cannot hold mail item
		}
	}

}
