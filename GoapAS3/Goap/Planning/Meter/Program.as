package Goap.Planning.Meter
{
	import Goap.IWriteState;

	/**
	 * @todo
	 */
	public class Program
	{
		public static const OP_NOP:String = "NOP";
		public static const OP_ADD:String = "ADD";
		public static const OP_SUB:String = "SUB";
		public static const OP_MUL:String = "MUL";
		public static const OP_DIV:String = "DIV";
		public static const OP_MOD:String = "MOD";
		public static const OP_POW:String = "POW";
		public static const OP_EQ:String  = "EQ";
		public static const OP_NEQ:String = "NEQ";
		public static const OP_LT:String  = "LT";
		public static const OP_GT:String  = "GT";
		public static const OP_LTE:String = "LTE";
		public static const OP_GTE:String = "GTE";
		public static const OP_AND:String = "AND";
		public static const OP_OR:String  = "OR";
		public static const OP_XOR:String = "XOR";
		public static const OP_NOT:String = "NOT";
		public static const OP_NEG:String = "NEG";
		public static const OP_INC:String = "INC";
		public static const OP_DEC:String = "DEC";
		public static const OP_BND:String = "BND";
		public static const OP_BOR:String = "BOR";
		public static const OP_BXR:String = "BXR";		
		public static const OP_BNT:String = "BNT";		
		public static const OP_LOAD:String = "LD";
		public static const OP_STORE:String = "ST";
		public static const OP_UNSET:String = "UNS";
		public static const OP_PUSH:String = "PUSH";
		
		public static const OP_END:String = "END";
		
		protected var _state:IWriteState; 
		protected var _program:Array = []; 
		protected var _stack:Array = [];
		
		public function Program()
		{
		}
		
		public function clear():void
		{
			_state = null;
			_program.length = 0;
			_stack.length = 0;
		}

		public function get program():Array
		{
			return _program;
		}

		public function get stack():Array
		{
			return _stack;
		}
		
		public function set stack(value:Array):void
		{
			_stack = value;
		}
		
		public function get state():IWriteState
		{
			return _state;
		}

		public function set state(value:IWriteState):void
		{
			_state = value;
		}

		///////////////////////////////////////////////////////////////////////////////////
		private var _pc:int;
		public function executeOp(op:Object):Boolean
		{
			var v1:*;
			var v2:*;
			var data:*;
			switch(op)
			{
				case OP_END:
					return false;
				case OP_LOAD:
					data = getOp() as String;
					data = _state.at(data);
					_stack.push(data);
					break;
				case OP_STORE:
					_state.put(getOp() as String, _stack.pop());
					break;
				case OP_UNSET:
					_state.remove(getOp() as String);
					break;
				case OP_PUSH:
					data = getOp();
					_stack.push(data);
					break;
				case OP_ADD:
					data = _stack.pop() + _stack.pop();
					_stack.push(data);
					break;
				case OP_SUB:
					data = _stack.pop() - _stack.pop();
					_stack.push(data);
					break;
				case OP_MUL:
					data = _stack.pop() * _stack.pop();
					_stack.push(data);
					break;
				case OP_DIV:
					data = _stack.pop() / _stack.pop();
					_stack.push(data);
					break;
				case OP_MOD:
					data = _stack.pop() % _stack.pop();
					_stack.push(data);
					break;
				case OP_POW:
					data = Math.pow(_stack.pop(), _stack.pop());
					_stack.push(data);
					break;
				case OP_EQ:
					data = _stack.pop() == _stack.pop();
					_stack.push(data);
					break;
				case OP_NEQ:
					data = _stack.pop() != _stack.pop();
					_stack.push(data);
					break;
				case OP_LT:
					data = _stack.pop() < _stack.pop();
					_stack.push(data);
					break;
				case OP_GT:
					data = _stack.pop() > _stack.pop();
					_stack.push(data);
					break;
				case OP_LTE:
					data = _stack.pop() <= _stack.pop();
					_stack.push(data);
					break;
				case OP_GTE:
					data = _stack.pop() >= _stack.pop();
					_stack.push(data);
					break;
				case OP_AND:
				{
					v1 = _stack.pop();
					v2 = _stack.pop();
					data = v1 && v2;
					_stack.push(data);
					break;
				}
				case OP_OR:
				{
					v1 = _stack.pop();
					v2 = _stack.pop();
					data = v1 || v2;
					_stack.push(data);
					break;
				}
				case OP_XOR:
				{
					v1 = _stack.pop();
					v2 = _stack.pop();
					data = (v1 && !v2) || (!v1 && v2);
					_stack.push(data);
					break;
				}
				case OP_NOT:
					data = !_stack.pop();
					_stack.push(data);
					break;
				case OP_NEG:
					data = -_stack.pop();
					_stack.push(data);
					break;
				case OP_INC:
					data = _stack.pop();
					_stack.push(++data);
					break;
				case OP_DEC:
					data = _stack.pop();
					_stack.push(--data);
					break;
				case OP_BND:
					data = _stack.pop() & _stack.pop();
					_stack.push(data);
					break;
				case OP_BOR:
					data = _stack.pop() | _stack.pop();
					_stack.push(data);
					break;
				case OP_BXR:
					data = _stack.pop() ^ _stack.pop();
					_stack.push(data);
					break;
				case OP_BNT:
					data = ~_stack.pop();
					_stack.push(data);
					break;
				case OP_NOP:
					break;
				default:
					throw new Error("Invalid OP");
			}
			return true;
		}
		
		[Inline]
		public final function getOp():Object
		{
			return _program[_pc++];
		}
		
		public function execute():Object
		{
			var ret:Object;
			var nIntructions:int = _program.length;
			_pc = 0;
			try {
				while (_pc < nIntructions) {
					if (!executeOp(getOp()))
						break;
				}
				ret = _stack.pop();
			} catch (e:Error) {
				trace ("[ConditionProgram] Error: " + e);
			}
			return ret;
		}
		
		///////////////////////////////////////////////////////////////////////////////////
		
		[Inline]
		public final function o(value:Object):Program
		{
			_program.push(value);
			return this;
		}

		
		
	}
}