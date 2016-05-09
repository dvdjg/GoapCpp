package Goap.Planning.Meter
{
	import Goap.Planning.IPlanningAction;

	/**
	 * Stores action information:
	 * - Preconditions: What are the simplest state expressions?
	 * - Modifications: What states changes and in what sense?
	 * @todo
	 */
	public class ActionInfo
	{
		// The changes done by this action
		public static const VALUE_SAME:int = 0;
		public static const VALUE_INCREMENT:int = 1; // For numerical values
		public static const VALUE_DECREMENT:int = 1; // For numerical values
		public static const VALUE_CHANGE:int = 1; // For boolean and string values
		
		protected var _action:IPlanningAction;
		
		public function ActionInfo(action_:IPlanningAction)
		{
			_action = action_;
		}

		public function get action():IPlanningAction
		{
			return _action;
		}

	}
}