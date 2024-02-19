#![allow(clippy::needless_bool)]
#![allow(unused_parens)]
#![allow(non_snake_case)]
#![allow(non_camel_case_types)]
#![allow(non_upper_case_globals)]

use std::io;

fn User_Input() -> [[u8; 9]; 9]
{
	let mut Initial_Puzzle: [[u8; 9]; 9] = [[0,0,0,0,0,0,0,0,0],
						[0,0,0,0,0,0,0,0,0],
						[0,0,0,0,0,0,0,0,0],
						[0,0,0,0,0,0,0,0,0],
						[0,0,0,0,0,0,0,0,0],
						[0,0,0,0,0,0,0,0,0],
						[0,0,0,0,0,0,0,0,0],
						[0,0,0,0,0,0,0,0,0],
						[0,0,0,0,0,0,0,0,0]];

	loop
	{
		println!("Enter the 81 digits of an unsolved classic sudoku puzzle in 9 lines. 0 counts as no clue.");
		for i in 0..9
		{
			let mut user_input: String = String::new();
			match io::stdin().read_line(&mut user_input)
			{
				Ok(_) =>
				{
					user_input = user_input.trim().to_string();
					for (j, character) in user_input.chars().take(9).enumerate()
					{
						match character.to_digit(10)
						{
							None => println!("Invalid character at position {}: {}. It will be treated as {}", j+1, character, Initial_Puzzle[i][j]),
							Some(digit) => Initial_Puzzle[i][j] = digit as u8,
						}
					}
				}
				Err(error) =>
				{
					println!("Error reading input: {}", error);
				}
			}
		}

		println!("Is this your puzzle? (Y/N)");
		Print_Puzzle(&Initial_Puzzle);
		let mut user_input: String = String::new();
		let mut Correct: bool = false;
		match io::stdin().read_line(&mut user_input)
		{
			Ok(_) =>
			{
				if let Some(character) = user_input.chars().next()
				{
					match character
					{
						'Y' | 'y' | '1' => Correct = true,
						_ => Correct = false,
					}
				}
				else
				{
					println!("You should have errored to 5 lines down");
				}
			}
			Err(error) =>
			{
				println!("Error reading input: {}", error);
			}
		}
		if(Correct)
		{
			break;
		}
	}

	return(Initial_Puzzle);
}

fn main()
{
	let Initial_Puzzle: [[u8; 9]; 9] = User_Input();

	let mut Current_Puzzle: Puzzle = Puzzle::new();
	Current_Puzzle.Initialize(Initial_Puzzle);
	let Possible_Solution: Option<Puzzle> = Current_Puzzle.Solve(0);

	let Final_Puzzle: Option<[[u8; 9]; 9]> = match Possible_Solution
	{
		None => None,
		_ => Some(Possible_Solution.clone().unwrap().Grid()),
	};

	match Final_Puzzle
	{
		None => (),
		_ => Print_Puzzle(&Final_Puzzle.unwrap()),
	};
	if(Possible_Solution != None && Possible_Solution.unwrap().Puzzle_Finished() == true)
	{
		println!("The solving algorithm has found a solution.");
	}
	else
	{
		println!("The solving algorithm has not found the solution.");
	}

}

#[derive(Clone,PartialEq)]
struct Puzzle
{
	data: [[[[u16; 3]; 3]; 3]; 3],
}

impl Puzzle
{
	fn new() -> Self
	{
		Self
		{
			data: [[[[511u16; 3]; 3]; 3]; 3],
		}
	}

	fn Initialize(&mut self, Init: [[u8; 9]; 9]) -> ()
	{
		for i in 0..9
		{
			for j in 0..9
			{
				self.data[i/3 as usize][j/3 as usize][i%3][j%3] = Self::Data_Encode(Init[i][j]);
			}
		}
	}

	fn Solve(&self, Depth: u64) -> Option<Puzzle>
	{
		let mut Previous_Step: Puzzle = self.clone();
		let mut Step: Puzzle = self.clone();
		loop
		{
			Previous_Step.Naked_Singles();
			Previous_Step.Hidden_Singles_Rows();
			Previous_Step.Hidden_Singles_Columns();
			Previous_Step.Hidden_Singles_Houses();
			if(Step.data == Previous_Step.data)
			{
				break;
			}
			Step.data = Previous_Step.data;
		}
		if(Previous_Step.Puzzle_Finished())
		{
			return(Some(Previous_Step));
		}
		Previous_Step.Thesus(Depth+1)
	}
	
	fn Thesus(&self, Depth: u64) -> Option<Puzzle>
	{
		let Singles: [u16; 9] = [1,2,4,8,16,32,64,128,256];
		for i in 0..3	//Puzzle row
		{
			for j in 0..3	//Puzzle column
			{
				for k in 0..3	//House row
				{
					for l in 0..3	//House column
					{
						if(!Singles.contains(&self.data[i][j][k][l]))
						{
							for num in 0..9	//iterate over the numbers
							{
								let bitmask: u16 = 1 << num;
								if(bitmask & self.data[i][j][k][l] != 0)
								{
									let mut Previous_Step: Puzzle = self.clone();
									Previous_Step.data[i][j][k][l] = bitmask;	//Set a possiblity
									let Next_Step: Option<Puzzle> = Previous_Step.Solve(Depth);	//Attempt a solve
									if(Next_Step != None && !Next_Step.clone().unwrap().Puzzle_Broke())	//return the solution if successful
									{
										return(Next_Step);
									}
								}
							}
							return(None);	//return none if not successful
						}
					}
				}
			}
		}
		return(Some(self.clone()));
	}

	fn Puzzle_Broke(&self) -> bool
	{
		for i in 0..3
		{
			for j in 0..3
			{
				for k in 0..3
				{
					for l in 0..3
					{
						if(self.data[i][j][k][l] == 0)
						{
							return(true);
						}
					}
				}
			}
		}
		false
	}

	fn Puzzle_Finished(&self) -> bool
	{
		for i in 0..3
		{
			for j in 0..3
			{
				for k in 0..3
				{
					for l in 0..3
					{
						match self.data[i][j][k][l]
						{
							1 | 2 | 4 | 8 | 16 | 32 | 64 | 128 | 256 => continue,
							_ => return(false),
						}
					}
				}
			}
		}
		true
	}

	fn Naked_Singles(&mut self) -> ()
	{
		let Singles: [u16; 9] = [1,2,4,8,16,32,64,128,256];
		for i in 0..3	//Puzzle row
		{
			for j in 0..3	//Puzzle column
			{
				for k in 0..3	//House row
				{
					for l in 0..3	//House column
					{
						if(Singles.contains(&self.data[i][j][k][l]))
						{
		for ip in 0..3	//Row navigation
		{
			for kp in 0..3
			{
				if(i != ip || k != kp)
				{
					self.data[ip][j][kp][l] = self.data[ip][j][kp][l] & !self.data[i][j][k][l];
				}
			}
		}
		for jp in 0..3	//Column navigation
		{
			for lp in 0..3
			{
				if(j != jp || l != lp)
				{
					self.data[i][jp][k][lp] = self.data[i][jp][k][lp] & !self.data[i][j][k][l];
				}
			}
		}
		for kp in 0..3	//House navigation
		{
			for lp in 0..3
			{
				if(k != kp || l != lp)
				{
					self.data[i][j][kp][lp] = self.data[i][j][kp][lp] & !self.data[i][j][k][l];
				}
			}
		}
						}
					}
				}
			}
		}
	}

	fn Hidden_Singles_Rows(&mut self) -> ()
	{
		for i in 0..9	//Rows
		{
			for j in 0..9	//Numbers
			{
				let bitmask: u16 = 1 << j;
				let mut First: bool = false;
				let mut Second: bool = false;
				let mut Position: usize = 0;

				for k in 0..9	//Columns
				{
					if(!First && (bitmask & self.data[i/3 as usize][k/3 as usize][i%3][k%3]) != 0)
					{
						First = true;
						Position = k;
					}
					else if(First && (bitmask & self.data[i/3 as usize][k/3 as usize][i%3][k%3]) != 0)
					{
						Second = true;
						break;
					}
				}

				if(First != Second)	//Hidden Single found, may be naked
				{
					for k in 0..9
					{
						self.data[i/3 as usize][k/3 as usize][i%3][k%3] = self.data[i/3 as usize][k/3 as usize][i%3][k%3] & !bitmask;
					}
					self.data[i/3 as usize][Position/3 as usize][i%3][Position%3] = bitmask;
				}
			}
		}
	}

	fn Hidden_Singles_Columns(&mut self) -> ()
	{
		for i in 0..9	//Columns
		{
			for j in 0..9	//Numbers
			{
				let bitmask: u16 = 1 << j;
				let mut First: bool = false;
				let mut Second: bool = false;
				let mut Position: usize = 0;

				for k in 0..9	//Rows
				{
					if(!First && (bitmask & self.data[k/3 as usize][i/3 as usize][k%3][i%3]) != 0)
					{
						First = true;
						Position = k;
					}
					else if(First && (bitmask & self.data[k/3 as usize][i/3 as usize][k%3][i%3]) != 0)
					{
						Second = true;
						break;
					}
				}

				if(First != Second)	//Hidden Single found, may be naked
				{
					for k in 0..9
					{
						self.data[k/3 as usize][i/3 as usize][k%3][i%3] = self.data[k/3 as usize][i/3 as usize][k%3][i%3] & !bitmask;
					}
					self.data[Position/3 as usize][i/3 as usize][Position%3][i%3] = bitmask;
				}
			}
		}
	}

	fn Hidden_Singles_Houses(&mut self) -> ()
	{
		for i in 0..9	//Houses
		{
			for j in 0..9	//Numbers
			{
				let bitmask: u16 = 1 << j;
				let mut First: bool = false;
				let mut Second: bool = false;
				let mut Position: usize = 0;

				for k in 0..9	//Cells
				{
					if(!First && (bitmask & self.data[i/3 as usize][i%3][k/3 as usize][k%3]) != 0)
					{
						First = true;
						Position = k;
					}
					else if(First && (bitmask & self.data[i/3 as usize][i%3][k/3 as usize][k%3]) != 0)
					{
						Second = true;
						break;
					}
				}

				if(First != Second)	//Hidden Single found, may be naked
				{
					for k in 0..9
					{
						self.data[i/3 as usize][i%3][k/3 as usize][k%3] = self.data[i/3 as usize][i%3][k/3 as usize][k%3] & !bitmask;
					}
					self.data[i/3 as usize][i%3][Position/3 as usize][Position%3] = bitmask;
				}
			}
		}
	}

	fn Grid(&self) -> [[u8; 9]; 9]
	{
		let mut Answer: [[u8; 9]; 9] = [[0u8; 9]; 9];
		for i in 0..9
		{
			for j in 0..9
			{
				Answer[i][j] = Puzzle::Data_Decode(self.data[i/3 as usize][j/3 as usize][i%3][j%3]);
			}
		}
		return(Answer);
	}

	fn Mix_Numbers(Row: &[u8; 9]) -> String
	{
		let mut Pipes: [u32; 19] = [0x2551,0x20,0x2502,0x20,0x2502,0x20,0x2551,0x20,0x2502,0x20,0x2502,0x20,0x2551,0x20,0x2502,0x20,0x2502,0x20,0x2551];
		for i in 0..9
		{
			if(Row[i] != 0)
			{
				Pipes[1+2*i] = (Row[i] as u32) + 48;
			}
		}
		Pipes.iter().filter_map(|&Pipes| std::char::from_u32(Pipes)).collect::<String>()
	}

	fn Data_Encode(Cell: u8) -> u16
	{
		match Cell
		{
			0 => 511,
			1 => 1,
			2 => 2,
			3 => 4,
			4 => 8,
			5 => 16,
			6 => 32,
			7 => 64,
			8 => 128,
			9 => 256,
			_ => 511,
		}
	}

	fn Data_Decode(Cell: u16) -> u8
	{
		match Cell
		{
			0 => 0,
			1 => 1,
			2 => 2,
			4 => 3,
			8 => 4,
			16 => 5,
			32 => 6,
			64 => 7,
			128 => 8,
			256 => 9,
			_ => 0,
		}
	}
}

fn Print_Puzzle(Puzzle: &[[u8; 9]; 9]) -> ()
{
	let Top_row: [u32; 19] = [0x2554,0x2550,0x2564,0x2550,0x2564,0x2550,0x2566,0x2550,0x2564,0x2550,0x2564,0x2550,0x2566,0x2550,0x2564,0x2550,0x2564,0x2550,0x2557];
	let Major_row: [u32; 19] = [0x2560,0x2550,0x256A,0x2550,0x256A,0x2550,0x256C,0x2550,0x256A,0x2550,0x256A,0x2550,0x256C,0x2550,0x256A,0x2550,0x256A,0x2550,0x2563];
	let Minor_row: [u32; 19] = [0x255F,0x2500,0x253C,0x2500,0x253C,0x2500,0x256B,0x2500,0x253C,0x2500,0x253C,0x2500,0x256B,0x2500,0x253C,0x2500,0x253C,0x2500,0x2562];
	let Bottom_row: [u32; 19] = [0x255A,0x2550,0x2567,0x2550,0x2567,0x2550,0x2569,0x2550,0x2567,0x2550,0x2567,0x2550,0x2569,0x2550,0x2567,0x2550,0x2567,0x2550,0x255D];

	println!("{}",Top_row.iter().filter_map(|&Top_row| std::char::from_u32(Top_row)).collect::<String>());

	let mut Number_row: String = Mix_Numbers(&Puzzle[0]);
	println!("{}", &Number_row);
	println!("{}",Minor_row.iter().filter_map(|&Minor_row| std::char::from_u32(Minor_row)).collect::<String>());
	Number_row = Mix_Numbers(&Puzzle[1]);
	println!("{}", &Number_row);
	println!("{}",Minor_row.iter().filter_map(|&Minor_row| std::char::from_u32(Minor_row)).collect::<String>());
	Number_row = Mix_Numbers(&Puzzle[2]);
	println!("{}", &Number_row);

	println!("{}",Major_row.iter().filter_map(|&Major_row| std::char::from_u32(Major_row)).collect::<String>());
	Number_row = Mix_Numbers(&Puzzle[3]);
	println!("{}", &Number_row);
	println!("{}",Minor_row.iter().filter_map(|&Minor_row| std::char::from_u32(Minor_row)).collect::<String>());
	Number_row = Mix_Numbers(&Puzzle[4]);
	println!("{}", &Number_row);
	println!("{}",Minor_row.iter().filter_map(|&Minor_row| std::char::from_u32(Minor_row)).collect::<String>());
	Number_row = Mix_Numbers(&Puzzle[5]);
	println!("{}", &Number_row);

	println!("{}",Major_row.iter().filter_map(|&Major_row| std::char::from_u32(Major_row)).collect::<String>());
	Number_row = Mix_Numbers(&Puzzle[6]);
	println!("{}", &Number_row);
	println!("{}",Minor_row.iter().filter_map(|&Minor_row| std::char::from_u32(Minor_row)).collect::<String>());
	Number_row = Mix_Numbers(&Puzzle[7]);
	println!("{}", &Number_row);
	println!("{}",Minor_row.iter().filter_map(|&Minor_row| std::char::from_u32(Minor_row)).collect::<String>());
	Number_row = Mix_Numbers(&Puzzle[8]);
	println!("{}", &Number_row);

	println!("{}",Bottom_row.iter().filter_map(|&Bottom_row| std::char::from_u32(Bottom_row)).collect::<String>());
}

fn Mix_Numbers(Row: &[u8; 9]) -> String
{
	let mut Pipes: [u32; 19] = [0x2551,0x20,0x2502,0x20,0x2502,0x20,0x2551,0x20,0x2502,0x20,0x2502,0x20,0x2551,0x20,0x2502,0x20,0x2502,0x20,0x2551];
	for i in 0..9
	{
		if(Row[i] != 0)
		{
			Pipes[1+2*i] = (Row[i] as u32) + 48;
		}
	}
	Pipes.iter().filter_map(|&Pipes| std::char::from_u32(Pipes)).collect::<String>()
}

impl std::fmt::Debug for Puzzle
{
	fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result
	{
		let mut Grid: [[u16; 9]; 9] = [[0u16; 9]; 9];
		for i in 0..9
		{
			for j in 0..9
			{
				Grid[i][j] = self.data[i/3 as usize][j/3 as usize][i%3][j%3];
			}
		}
		for i in 0..8
		{
			write!(f, "{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}\n",Grid[i][0],Grid[i][1],Grid[i][2],Grid[i][3],Grid[i][4],Grid[i][5],Grid[i][6],Grid[i][7],Grid[i][8])?;
		}
		write!(f, "{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}",Grid[8][0],Grid[8][1],Grid[8][2],Grid[8][3],Grid[8][4],Grid[8][5],Grid[8][6],Grid[8][7],Grid[8][8])
	}
}

impl std::fmt::Display for Puzzle
{
	fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result
	{
		let mut Grid: [[u8; 9]; 9] = [[0u8; 9]; 9];
		let Top_row: [u32; 19] = [0x2554, 0x2550, 0x2564, 0x2550, 0x2564, 0x2550, 0x2566, 0x2550, 0x2564, 0x2550, 0x2564, 0x2550, 0x2566, 0x2550, 0x2564, 0x2550, 0x2564, 0x2550, 0x2557];
		let Major_row: [u32; 19] = [0x2560, 0x2550, 0x256A, 0x2550, 0x256A, 0x2550, 0x256C, 0x2550, 0x256A, 0x2550, 0x256A, 0x2550, 0x256C, 0x2550, 0x256A, 0x2550, 0x256A, 0x2550, 0x2563];
		let Minor_row: [u32; 19] = [0x255F, 0x2500, 0x253C, 0x2500, 0x253C, 0x2500, 0x256B, 0x2500, 0x253C, 0x2500, 0x253C, 0x2500, 0x256B, 0x2500, 0x253C, 0x2500, 0x253C, 0x2500, 0x2562];
		let Bottom_row: [u32; 19] = [0x255A, 0x2550, 0x2567, 0x2550, 0x2567, 0x2550, 0x2569, 0x2550, 0x2567, 0x2550, 0x2567, 0x2550, 0x2569, 0x2550, 0x2567, 0x2550, 0x2567, 0x2550, 0x255D];

		for i in 0..9
		{
			for j in 0..9
			{
				Grid[i][j] = Puzzle::Data_Decode(self.data[i/3 as usize][j/3 as usize][i%3][j%3]);
			}
		}

		write!(f, "{}\n", Top_row.iter().filter_map(|&Top_row| std::char::from_u32(Top_row)).collect::<String>())?;

		let mut Number_row: String = Puzzle::Mix_Numbers(&Grid[0]);
		write!(f, "{}\n", &Number_row)?;
		write!(f, "{}\n",Minor_row.iter().filter_map(|&Minor_row| std::char::from_u32(Minor_row)).collect::<String>())?;
		Number_row = Puzzle::Mix_Numbers(&Grid[1]);
		write!(f, "{}\n", &Number_row)?;
		write!(f, "{}\n",Minor_row.iter().filter_map(|&Minor_row| std::char::from_u32(Minor_row)).collect::<String>())?;
		Number_row = Puzzle::Mix_Numbers(&Grid[2]);
		write!(f, "{}\n", &Number_row)?;

		write!(f, "{}\n",Major_row.iter().filter_map(|&Major_row| std::char::from_u32(Major_row)).collect::<String>())?;
		Number_row = Puzzle::Mix_Numbers(&Grid[3]);
		write!(f, "{}\n", &Number_row)?;
		write!(f, "{}\n",Minor_row.iter().filter_map(|&Minor_row| std::char::from_u32(Minor_row)).collect::<String>())?;
		Number_row = Puzzle::Mix_Numbers(&Grid[4]);
		write!(f, "{}\n", &Number_row)?;
		write!(f, "{}\n",Minor_row.iter().filter_map(|&Minor_row| std::char::from_u32(Minor_row)).collect::<String>())?;
		Number_row = Puzzle::Mix_Numbers(&Grid[5]);
		write!(f, "{}\n", &Number_row)?;

		write!(f, "{}\n",Major_row.iter().filter_map(|&Major_row| std::char::from_u32(Major_row)).collect::<String>())?;
		Number_row = Puzzle::Mix_Numbers(&Grid[6]);
		write!(f, "{}\n", &Number_row)?;
		write!(f, "{}\n",Minor_row.iter().filter_map(|&Minor_row| std::char::from_u32(Minor_row)).collect::<String>())?;
		Number_row = Puzzle::Mix_Numbers(&Grid[7]);
		write!(f, "{}\n", &Number_row)?;
		write!(f, "{}\n",Minor_row.iter().filter_map(|&Minor_row| std::char::from_u32(Minor_row)).collect::<String>())?;
		Number_row = Puzzle::Mix_Numbers(&Grid[8]);
		write!(f, "{}\n", &Number_row)?;

		write!(f, "{}",Bottom_row.iter().filter_map(|&Bottom_row| std::char::from_u32(Bottom_row)).collect::<String>())
	}
}
