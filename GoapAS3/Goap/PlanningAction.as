package Goap
{
	import Goap.Planning.IPlanningAction;

	/**
	 * Action
	 * 
	 * An action is a single, atomic step within a plan that makes a character do something.
	 * Some possible actions include GotoPoint, ActivateObject, DrawWeapon, 
	 * ReloadWeapon, and Attack. The duration of an action may be short or infinitely 
	 * long. The ReloadWeaponaction will complete as soon as the character finishes a 
	 * reload animation. The Attackaction may continue infinitely, until the target is dead.
	 * 
	 * Each action knows when it is valid to run, and what it will do to the game world. In other 
	 * words, an action knows its preconditionsand effects. Preconditions and effects provide a 
	 * mechanism for chaining actions into a valid sequence. For example, Attackhas a 
	 * precondition that the character’s weapon is loaded. The effect of ReloadWeaponis that 
	 * the weapon is loaded. It is east to see that ReloadWeaponfollowed by Attackis a 
	 * valid sequence of actions. Each action may have any number of preconditions and 
	 * effects.
	 */
	public final class PlanningAction implements IPlanningAction
	{
		public static var pool:ObjectPool = ObjectPool.getInstance(PlanningAction);
		
		public static function getNew(name:String, validator:Function, executor:Function):PlanningAction
		{
			var action:PlanningAction = pool.getNew();
			action._name = name;
			action._validator = validator;
			action._executor = executor;
			return action;
		}
		
		private var _name:String;
		private var _validator:Function; // State -> bool. Preconditions. The input State is not modified.
		private var _executor:Function; // State -> void. Effects. The input State is modified.

		public function PlanningAction(name_:String=null, validator_:Function=null, executor_:Function=null)
		{
			_name = name_;
			_validator = validator_;
			_executor = executor_;
		}
		
		[Inline]
		public final function get executor():Function
		{
			return _executor;
		}

		[Inline]
		public final function get validator():Function
		{
			return _validator;
		}

		[Inline]
		public final function get name():String
		{
			return _name;
		}
		
		/**
		 * Returns true if the action can be executed given this input state.
		 */
		[Inline]
		public final function canExecute(state:IReadState):Boolean
		{
			return _validator(state);
		}
		
		/**
		 * Returns a modified cloned state. The input state is not modified.
		 */
		[Inline]
		public final function execute(state:IReadState):IWriteState
		{
			// The executor evolves input state to a new state, so we need to pass a copy to preserve the original
			var newState:IWriteState = state.clone();
			_executor(newState);
			return newState;
		}
		
		[Inline]
		public final function toString():String {
			return _name;
		}
	}	
}
