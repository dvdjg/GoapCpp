package
{
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.KeyboardEvent;
	import flash.events.MouseEvent;
	
	import Goap.State;
	import Goap.Planning.Meter.Program;
	
	import Tests.PlannerTests.backing_a_pie;
	import Tests.PlannerTests.hanoi_tower_solver_test;
	
	public class GOAP extends Sprite
	{
		public function GOAP()
		{
			addEventListener(Event.ADDED_TO_STAGE, init);
		}
		
		public function init(e:Event):void 
		{
			stage.addEventListener(MouseEvent.CLICK, onMouseClickEvent);
			stage.addEventListener(KeyboardEvent.KEY_DOWN, onKeyClickEvent);
		}
		
		// then make the callback
		public function onKeyClickEvent(event:KeyboardEvent):void 
		{
			hanoi.run2();
			return;
			
			var exec:Object;
			var state:State = State.getNew({A:10, B:true});
			var prog:Program = new Program;
			prog.state = state;
			prog.o(Program.OP_PUSH).o(8).o(Program.OP_LOAD).o("A").o(Program.OP_ADD).o(Program.OP_PUSH).o(18).o(Program.OP_GT);
			exec = prog.execute();
			trace("[Program] res =", exec); 
			
			prog.o(Program.OP_PUSH).o(27).o(Program.OP_PUSH).o(12).o(Program.OP_PUSH).o(1).o(Program.OP_PUSH).o(2)
				.o(Program.OP_ADD).o(Program.OP_MUL).o(Program.OP_ADD);
			exec = prog.execute();
			trace("[Program] res =", exec); // 63
			
			trace(event);

		}
		protected var backing:backing_a_pie = new backing_a_pie;
		protected var hanoi:hanoi_tower_solver_test = new hanoi_tower_solver_test;
		// then make the callback
		public function onMouseClickEvent(event:MouseEvent):void 
		{
			trace(event);
			backing.run();
			hanoi.run();
		}
	}
}