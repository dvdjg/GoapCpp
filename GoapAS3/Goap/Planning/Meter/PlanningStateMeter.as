package Goap.Planning.Meter
{
	import Goap.IReadState;
	
	/**
	 * Helper class for measuring the distance from a given state to the goal state.
	 * This class must be overrided to implement valid measuring functions.
	 */
	public class PlanningStateMeter implements IPlanningStateMeter
	{
		protected var _goalState:IReadState;
		private var _isMonotonic:Boolean = false;
		
		public function PlanningStateMeter(goalState:IReadState)
		{
			_goalState = goalState;
		}
		
		/**
		 * Returns true when the input 'state' satisfies the goal state.
		 */
		public function enough(state:IReadState):Boolean
		{
			return distance(state) == 0;
		}
		
		/**
		 * Returns the measure from the input state to the goal state
		 */
		public function distance(state:IReadState):Number
		{
			return 0;
		}

		[Inline]
		public final function get goalState():IReadState
		{
			return _goalState;
		}
		
		[Inline]
		public final function get monotonic():Boolean
		{
			return _isMonotonic;
		}
		
		[Inline]
		public final function set monotonic(monotonic:Boolean):void
		{
			_isMonotonic = monotonic;
		}
	}
}