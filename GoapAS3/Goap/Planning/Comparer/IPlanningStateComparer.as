package Goap.Planning.Comparer
{
	import Goap.IReadState;
	
	public interface IPlanningStateComparer
	{
		/**
		 * Returns true when state1 is similar enough to state2 for stopping the search.
		 */
		function enough(state1:IReadState, state2:IReadState):Boolean;
		function distance(state1:IReadState, state2:IReadState):Number;
		
//		/**
//		 * If the used algorithm could not be sure that the distance does not generates local minimum values.
//		 */
//		function get monotonic():Boolean;
//		function set monotonic(b:Boolean):void;
	}	
}

