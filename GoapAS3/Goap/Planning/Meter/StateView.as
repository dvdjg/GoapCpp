package Goap.Planning.Meter
{
	import Goap.IReadState;

	/**
	 * High level state composed from a combination of other states.
	 * @todo
	 */
	public class StateView
	{
		protected var _state:IReadState;
		public function StateView(state_:IReadState = null)
		{
			_state = state_;
		}

		public function get state():IReadState
		{
			return _state;
		}

		public function set state(value:IReadState):void
		{
			_state = value;
		}

		public function get value():int {
			return 0;
		}
	}
}