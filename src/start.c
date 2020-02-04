#include "head.h"

void	get_oper_code(t_cursor *cursor, unsigned char *arena)
{
	static int count = 0;

	if (cursor->cycles_before_op == 0)
	{
		cursor->op_code = arena[cursor->cur_position] - 1;
		if (cursor->op_code >= 0x01 && cursor->op_code <= 0x10)
		{
			count = 0;
			ft_printf("OPER: %s\n", op_tab[cursor->op_code].name);
			cursor->cycles_before_op = op_tab[cursor->op_code].cycles_to_exec;
		}
		else
		{
			count++;
			cursor->cycles_before_op = 0;
			ft_printf("OPCODE IS BAD count: %d\n", count);
		}
	}
}

void	decrease_cycle_before_op(t_cursor *cursor)
{
	if (cursor->cycles_before_op > 0)
		cursor->cycles_before_op--;
}

void	exec_oper(t_cursor *cursor, unsigned char *arena, void (*do_oper)(t_cursor*, unsigned char*)) // 	NEED CHECK FOR EVERY OPERATION
{
	if (!(cursor->op_code >= 0x01 && cursor->op_code <= 0x10))
	{
		ft_printf("BAD OPCODE: pos + 1\n");
		cursor->cur_position = (cursor->cur_position + 1) % MEM_SIZE;
		ft_printf("cur_pos = %d\n", cursor->cur_position);
		return ;
	}
	// THIS if-esle FOR GETIING cursor->arg ONLY
	if (op_tab[cursor->op_code].code_type_args) // 1 = exist, 0 - non
	{
		ft_printf("ARG CODE: %08b(bin) %d(dec)\n", arena[cursor->cur_position + 1], arena[cursor->cur_position + 1]);
		cursor->arg_type[0] = (arena[cursor->cur_position + 1] & 0b11000000) >> 6; // EBUCHAYA MASKA
		cursor->arg_type[1] = (arena[cursor->cur_position + 1] & 0b00110000) >> 4; // now MUST work
		cursor->arg_type[2] = (arena[cursor->cur_position + 1] & 0b00001100) >> 2; // kill yourself if not

		ft_printf("[1]: %02b\n", cursor->arg_type[0]);
		ft_printf("[2]: %02b\n", cursor->arg_type[1]);
		ft_printf("[3]: %02b\n", cursor->arg_type[2]);

	}
	else
	{
		ft_printf("NO ARG CODE\n");
		cursor->arg_type[0] = op_tab[cursor->op_code].arg[0]; //  -- T_REG or T_DIR
		ft_printf("ARG = %2b\n", cursor->arg_type[0]);
	}
	
	// THIS FOR CHECKING TYPES OF ARGUMENTS FROM op_tab and for register numbers
	if (!check_arg_type(cursor) || !check_registers(cursor, arena)) // checking for every operation IM GENIUS
	{
		exit(0);
		ft_printf("NOT GUT ARG TYPES\n");
	}
	//if checking is ok, then do the operation
	else
	{
		do_oper(cursor, arena);  // do oper
				// print_arena_2(arena);
		exit(0);
	}


	cursor->cur_position = (cursor->cur_position + cursor->bytes_to_next_op) % MEM_SIZE;
	printf("CURRENT POSITION: %zu\n", cursor->cur_position);
	printf("BYTECODE: %02x\n", arena[cursor->cur_position]);
	// else
	// 	// change cur_position
}

void	start(t_vm *vm)
{
	t_cursor *cursor;

	vm->cursor = init_first_cursors(vm);

	// print_list_of_cursors(vm->cursor);
	// printf("NUM OF CURSORS: %d\n", vm->num_of_cursors);

	cursor = vm->cursor;
	while (1)
	{
		while (cursor)
		{   //----------IN CYCLE
			
			// ft_printf("cursor ID: %d\n", cursor->id);
			get_oper_code(cursor, vm->arena);
			decrease_cycle_before_op(cursor);
			if (cursor->cycles_before_op == 0)
				exec_oper(cursor, vm->arena, vm->do_oper[cursor->op_code]); // do_oper is operation №op_code from operations.c
			cursor = cursor->next;
		}
		if ((vm->cycles_to_die > 0 && vm->num_of_cycles == vm->cycles_to_die)
			|| vm->cycles_to_die <= 0)
			inspection(vm, vm->cursor);
		cursor = vm->cursor;
	}
}
