package Goap.Planning.Meter
{
	import Goap.IReadState;
	import Goap.Planning.Comparer.IPlanningStateComparer;
	import Goap.Planning.Comparer.NumericStateComparer;
	
	public class ComparerStateMeter extends PlanningStateMeter
	{
		protected var _comparer:IPlanningStateComparer;
		public function ComparerStateMeter(goalState:IReadState, stateComparer:IPlanningStateComparer = null)
		{
			super(goalState);
			if (stateComparer == null)
				stateComparer = NumericStateComparer.singleton;
			_comparer = stateComparer;
		}
		
		[Inline]
		public final override function distance(state:IReadState):Number
		{
			return _comparer.distance(state, _goalState);
		}
		
		[Inline]
		public final override function enough(state:IReadState):Boolean
		{
			return _comparer.enough(state, _goalState);
		}

		[Inline]
		public final function get comparer():IPlanningStateComparer
		{
			return _comparer;
		}

		[Inline]
		public final function set comparer(value:IPlanningStateComparer):void
		{
			_comparer = value;
		}
	}
}