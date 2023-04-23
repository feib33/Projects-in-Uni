/**
 * SWEN30006 Project A
 * @author Fei BAO
 * @studentID 804522
 */
package strategies;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

import automail.MailItem;
import automail.PriorityMailItem;
import automail.StorageTube;
import exceptions.TubeFullException;

public class MyMailPool implements IMailPool{
	
	private List<MailItem> nonPriorityPool;
	private List<PriorityMailItem> priorityPool;
	private static final int MAX_TAKE = 4;
	
	public MyMailPool(){
		// Start empty
		nonPriorityPool = new ArrayList<MailItem>();
		priorityPool = new ArrayList<PriorityMailItem>();
	}
	
	/**
	 * Override the comparator for sorting priorityPool by priority level 
	 * in descending at first and then by floor number in ascending
	 */
	public static Comparator<PriorityMailItem> priorityComparator = new Comparator<PriorityMailItem>() {

		@Override
		public int compare(PriorityMailItem p1, PriorityMailItem p2) {
			int priorityLvl1 = p1.getPriorityLevel();
			int priorityLvl2 = p2.getPriorityLevel();
			
			//sort by floor number when priority levels are same 
			if(priorityLvl1 == priorityLvl2) {
				int floor1 = p1.getDestFloor();
				int floor2 = p2.getDestFloor();
				
				return floor1 - floor2;
			}else {
				return priorityLvl2 - priorityLvl1;
			}
			
		}
		
	};
	
	/**
	 * Override the comparator for sorting nonPriorityPool by floor number 
	 * in ascending at first and then by arrival time in ascending
	 */
	public static Comparator<MailItem> mailComparator = new Comparator<MailItem>() {

		@Override
		public int compare(MailItem m1, MailItem m2) {
			int floor1 = m1.getDestFloor();
			int floor2 = m2.getDestFloor();
			
			//sort by arrival time when they are on same floor
			if(floor1 == floor2) {
				int arri1 = m1.getArrivalTime();
				int arri2 = m2.getArrivalTime();
				
				return arri1 - arri2;
			} else {
				return floor1 - floor2;
			}
		}
		
	};
	
	
	@Override
	public void addToPool(MailItem mailItem) {
		if(mailItem instanceof PriorityMailItem){
			//Add priority mail item into priority pool and sort pool
			priorityPool.add((PriorityMailItem) mailItem);
			Collections.sort(priorityPool, MyMailPool.priorityComparator);
		}
		else {
			//Add normal mail item into non-priority pool and sort pool
			nonPriorityPool.add(mailItem);
			Collections.sort(nonPriorityPool, MyMailPool.mailComparator);
		}
	}
	
	/**
	 * Get the size of nonPriorityPool
	 * @return
	 */
	private int getNonPriorityPoolSize() {
		return nonPriorityPool.size();
	}
	
	/**
	 * Get the size of priorityPool
	 * @return
	 */
	private int getPriorityPoolSize(){
		return priorityPool.size();
	}
	
	
	/**
	 * Get the first priority mail item in the pool
	 * @param weightLimit refers the maximum weight robot can hold
	 * @return first mail in priority pool
	 */
	public MailItem getHighestPriorityMail(int weightLimit) {
		for(int i = 0; i < priorityPool.size(); ++i) {
			if(priorityPool.get(i).getWeight() <= weightLimit) {
				return priorityPool.remove(i);
			}
		}
		return null;
	}
	
	/**
	 * Get the first normal mail item in the pool
	 * @param weightLimit refers the maximum weight robot can hold
	 * @return first normal mail in non-priority pool
	 */
	public MailItem getNonPriorityMail(int weightLimit) {
		for(int i = 0; i < nonPriorityPool.size(); ++i) {
			if(nonPriorityPool.get(i).getWeight() <= weightLimit) {
				return nonPriorityPool.remove(i);
			}
		}
		return null;
	}
	
	/**
	 * Remove null in the tube
	 * @param tube refers the storage unit
	 */
	public void removeNull(StorageTube tube) {
		MailItem topMail = tube.peek();
		if(topMail == null) {
			tube.pop();
		}
	}
	
	@Override
	public void fillStorageTube(StorageTube tube, boolean strong) {
		int max = strong ? Integer.MAX_VALUE : 2000;
		try{
			// Start afresh by emptying undelivered items back in the pool
			while(!tube.isEmpty()) {
				addToPool(tube.pop());
			}
			
			if (getPriorityPoolSize() > 0) {
				// Add priority mail item
				tube.addItem(getHighestPriorityMail(max));
				removeNull(tube); // Remove mail in the tube if it is null
			}
			// Get as many nonpriority items as available or as fit
			while(tube.getSize() < MAX_TAKE && getNonPriorityPoolSize() > 0) {
				tube.addItem(getNonPriorityMail(max));
				removeNull(tube);
				
				//let weak robot give up when there is only one mail that weak robot cannot hold
				if(getNonPriorityPoolSize() == 1 && tube.getSize() == 0) {
					break;
				}
			}
			
			
		}
		catch(TubeFullException e){
			e.printStackTrace();
		}
	}

}
