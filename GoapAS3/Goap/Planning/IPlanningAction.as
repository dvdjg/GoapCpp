package Goap.Planning
{
	import Goap.IReadState;
	import Goap.IWriteState;
	
	public interface IPlanningAction
	{
		function get name():String;
		/**
		 * Returns true if the action can be executed given this input state.
		 */
		function canExecute(state:IReadState):Boolean;
		/**
		 * Returns a modified cloned state. The input state is not modified.
		 */
		function execute(state:IReadState):IWriteState;
	}	
}
