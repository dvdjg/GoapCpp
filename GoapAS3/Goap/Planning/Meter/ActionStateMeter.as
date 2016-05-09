package Goap.Planning.Meter
{
	import Goap.IReadState;
	import Goap.State;
	import Goap.Planning.IPlanningAction;
	
	/**
	 * This class generates the 'distance' and 'enough' functions according the available actions (world rules) and goal state.
	 * @todo all
	 */
	public class ActionStateMeter extends PlanningStateMeter
	{
		protected var _actions:Vector.<ActionInfo>; // The world rules
		protected var _sampleState:IReadState;
		protected static const _emptyState:IReadState = new State();
		
		public function ActionStateMeter(goalState:IReadState, actions:Vector.<IPlanningAction>=null)
		{
			super(goalState);
			_sampleState = State.getNew();
			for each(var action:IPlanningAction in actions) {
				_actions.push(new ActionInfo(action));	
			}
			run();
		}
		
		[Inline]
		public final function get actions():Vector.<ActionInfo>
		{
			return _actions;
		}
		
		
		protected function run():void
		{
			for each(var action:ActionInfo in _actions) {
				
			}
			
		}
	}
}