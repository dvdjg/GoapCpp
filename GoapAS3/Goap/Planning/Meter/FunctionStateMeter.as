package Goap.Planning.Meter
{
	import Goap.IReadState;
	import Goap.Planning.Comparer.ExactStateComparer;
	import Goap.Planning.Comparer.NumericStateComparer;
	
	public class FunctionStateMeter extends PlanningStateMeter
	{
		private var _fnDistance:Function; // Signature: function(state:State, stateMeter:FunctionStateMeter):Number
		private var _fnEnough:Function; // Signature: function(state:State, stateMeter:FunctionStateMeter):Boolean
		private var _stateMeters:Object = {};
		private var _numericStateMeter:ComparerStateMeter;
		private var _exactStateMeter:ComparerStateMeter;
		
		public function FunctionStateMeter(goalState:IReadState)
		{
			super(goalState);
			_numericStateMeter = new ComparerStateMeter(goalState, NumericStateComparer.singleton);
			addStateMeter("numeric", _numericStateMeter);
			_exactStateMeter = new ComparerStateMeter(goalState, ExactStateComparer.singleton);
			_exactStateMeter.monotonic = true;
			addStateMeter("exact", _exactStateMeter);
			
			_fnDistance = function (state:IReadState, stateMeter:FunctionStateMeter):Number 
			{
				return stateMeter.numericStateMeter.distance(state);
			};
			_fnEnough = function (state:IReadState, stateMeter:FunctionStateMeter):Boolean 
			{
				return stateMeter.numericStateMeter.enough(state);
			};
		}
		
		[Inline]
		public final override function distance(state:IReadState):Number
		{
			return _fnDistance(state, this);
		}
		
		[Inline]
		public final override function enough(state:IReadState):Boolean
		{
			return _fnEnough(state, this);
		}
		
		[Inline]
		public final function addStateMeter(name:String, stateMeter:IPlanningStateMeter):void
		{
			_stateMeters[name] = stateMeter;
		}
		
		[Inline]
		public final function getStateMeter(name:String):IPlanningStateMeter
		{
			return _stateMeters[name];
		}

		[Inline]
		public final function get fnDistance():Function
		{
			return _fnDistance;
		}

		// Signature: function(state:State, stateMeter:FunctionStateMeter):Number
		[Inline]
		public final function set fnDistance(value:Function):void
		{
			_fnDistance = value;
		}

		[Inline]
		public final function get fnEnough():Function
		{
			return _fnEnough;
		}

		// Signature: function(state:State, stateMeter:FunctionStateMeter):Boolean
		[Inline]
		public final function set fnEnough(value:Function):void
		{
			_fnEnough = value;
		}

		[Inline]
		public final function get numericStateMeter():ComparerStateMeter
		{
			return _numericStateMeter;
		}

		[Inline]
		public final function get exactStateMeter():ComparerStateMeter
		{
			return _exactStateMeter;
		}


	}
}