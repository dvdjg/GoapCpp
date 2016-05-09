package Goap
{
	import Goap.Planning.IPlanningAction;

	public class StatesPlan
	{
		protected var _states:Vector.<IReadState> = new Vector.<IReadState>;
		protected var _plan:Vector.<IPlanningAction> = new Vector.<IPlanningAction>;
		public function StatesPlan()
		{
		}

		[Inline]
		public final function get states():Vector.<IReadState>
		{
			return _states;
		}

		[Inline]
		public final function set states(value:Vector.<IReadState>):void
		{
			_states = value;
		}

		[Inline]
		public final function get plan():Vector.<IPlanningAction>
		{
			return _plan;
		}

		[Inline]
		public final function set plan(value:Vector.<IPlanningAction>):void
		{
			_plan = value;
		}


	}
}