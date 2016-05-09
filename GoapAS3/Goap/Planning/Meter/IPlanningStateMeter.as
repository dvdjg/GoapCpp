package Goap.Planning.Meter
{
	import Goap.IReadState;

	public interface IPlanningStateMeter
	{
		/**
		 * Returns true when the state has reach the goal, so the input 'state' satisfies the goal state.
		 */
		function enough(state:IReadState):Boolean;
		/**
		 * Returns the measure from the input state to the goal state.
		 * It will return lower numbers when it deems that the goal is close so the planner can direct the search.
		 */
		function distance(state:IReadState):Number;
		
		/**
		 * If true, the distance functions gives hints for Landmarkable search:
		 * The distance supplied gives strong suggestions. It really means the real distance to the result.
		 * 
		 * If the distante function gives only vage hints (the typical case), monotonic must be false.
		 */
		function get monotonic():Boolean;
		
		function get goalState():IReadState;
	}
}