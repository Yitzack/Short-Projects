#![allow(clippy::needless_bool)]
#![allow(unused_parens)]
#![allow(non_snake_case)]
#![allow(non_camel_case_types)]
#![allow(non_upper_case_globals)]

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

fn main()
{
	let Initial_Puzzle: [[u8; 9]; 9] = [[0,2,7,0,8,0,0,0,0],
					   [0,0,0,0,0,0,6,0,1],
					   [0,0,0,0,0,0,0,0,0],
					   [6,0,0,5,0,1,0,0,0],
					   [0,0,3,0,0,0,0,8,0],
					   [0,0,0,0,0,0,0,0,0],
					   [0,0,0,3,7,0,0,2,0],
					   [9,1,0,0,0,0,4,0,0],
					   [0,0,0,8,0,0,0,0,0]];

	let mut Current_Puzzle: Puzzle = Puzzle::new();
	Current_Puzzle.Initialize(Initial_Puzzle);

	let Solution: [[u8; 9]; 9] = [[4,2,7,1,8,6,3,5,9],
				      [8,9,5,2,4,3,6,7,1],
				      [3,6,1,7,9,5,8,4,2],
				      [6,8,4,5,2,1,7,9,3],
				      [1,5,3,9,6,7,2,8,4],
				      [2,7,9,4,3,8,5,1,6],
				      [5,4,6,3,7,9,1,2,8],
				      [9,1,8,6,5,2,4,3,7],
				      [7,3,2,8,1,4,9,6,5]];

	Print_Puzzle(&Initial_Puzzle);
	Print_Puzzle(&Solution);
	println!("{}", Current_Puzzle);
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
