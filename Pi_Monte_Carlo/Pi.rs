#![allow(clippy::needless_bool)]
#![allow(unused_parens)]
#![allow(non_snake_case)]
#![allow(non_camel_case_types)]
#![allow(non_upper_case_globals)]

use std::time::{SystemTime, UNIX_EPOCH};
use std::f64::consts::PI;

const NUM_THREADS: usize = 24;

#[derive(Clone)]
struct ThreadData
{
	Points_in_Sphere: Vec<u64>,
	Samples_Used: Vec<u64>,
}

fn worker_thread(thread_id: usize, Data: &mut Vec<ThreadData>, RNG: &mut Mersenne_Twist) -> ()
{
	let mut Sample: Vec<f64> = Vec::with_capacity(2520);
	let Sub_Samples: Vec<u16> = vec![1260,840,630,504,420,360,315,280,252];

	for _ in 0..2520
	{
		Sample.push(Uniform(RNG.rand().expect("Unseeded RNG")));
	}

	for N in 2..=10
	{
		Data[thread_id].Points_in_Sphere[N-2] += Samples_in_Sphere(&Sample, N as u64);
		Data[thread_id].Samples_Used[N-2] += Sub_Samples[N-2] as u64;
	}
}

fn main()
{
	let mut Data: Vec<ThreadData> = vec![ThreadData{Points_in_Sphere: vec![0;9], Samples_Used: vec![0;9]}; NUM_THREADS];
	let mut pi: Vec<Vec<f64>> = vec![vec![0.0f64; NUM_THREADS]; 9];
	let mut Mean: Vec<f64> = vec![0.0f64;10];
	let mut StdDev: Vec<f64> = vec![0.0f64;10];
	let mut RNG: Vec<Mersenne_Twist> = vec![Mersenne_Twist::new(); NUM_THREADS];

	for i in 0..24
	{
		RNG[i].srand(SystemTime::now().duration_since(UNIX_EPOCH).expect("System Error").as_secs() + (i as u64));
	}

	loop
	{
		for thread_id in 0..NUM_THREADS
		{
			worker_thread(thread_id, &mut Data, &mut RNG[thread_id]);
		}

		if((Data[0].Samples_Used[8]/252)%1000==999)
		{
			for i in 0..NUM_THREADS
			{
				pi[0][i] = 4.0f64*(Data[i].Points_in_Sphere[0] as f64)/(Data[i].Samples_Used[0] as f64);
				pi[1][i] = 6.0f64*(Data[i].Points_in_Sphere[1] as f64)/(Data[i].Samples_Used[1] as f64);
				pi[2][i] = 4.0f64*(2.0f64*(Data[i].Points_in_Sphere[2] as f64)/(Data[i].Samples_Used[2] as f64)).sqrt();
				pi[3][i] = 2.0f64*(15.0f64*(Data[i].Points_in_Sphere[3] as f64)/(Data[i].Samples_Used[3] as f64)).sqrt();
				pi[4][i] = 4.0f64*(6.0f64*(Data[i].Points_in_Sphere[4] as f64)/(Data[i].Samples_Used[4] as f64)).powf(1.0f64/3.0f64);
				pi[5][i] = 2.0f64*(105.0f64*(Data[i].Points_in_Sphere[5] as f64)/(Data[i].Samples_Used[5] as f64)).powf(1.0f64/3.0f64);
				pi[6][i] = 4.0f64*(24.0f64*(Data[i].Points_in_Sphere[6] as f64)/(Data[i].Samples_Used[6] as f64)).powf(0.25f64);
				pi[7][i] = 2.0f64*(945.0f64*(Data[i].Points_in_Sphere[7] as f64)/(Data[i].Samples_Used[7] as f64)).powf(0.25f64);
				pi[8][i] = 4.0f64*(120.0f64*(Data[i].Points_in_Sphere[8] as f64)/(Data[i].Samples_Used[8] as f64)).powf(0.2f64);
			}

			for i in 0..9
			{
				Mean[i] = mean(&pi[i], NUM_THREADS).expect("Insufficent data");
				StdDev[i] = stddev(&pi[i], NUM_THREADS).expect("Insufficent data");
			}
			Mean[9] = mean(&Mean, 9 as usize).expect("Insufficent data");
			StdDev[9] = meanDev(&StdDev, 9 as usize).expect("Insufficent data");

			print!("{},{},{},{},{},{},{},{},{},{},Around[{},{}],",Data[0].Samples_Used[8]+1, Mean[0], Mean[1], Mean[2], Mean[3], Mean[4], Mean[5], Mean[6], Mean[7], Mean[8], Mean[9], StdDev[9]);

			if(Mean[9]-StdDev[9] < PI && Mean[9]+StdDev[9] > PI)
			{
				println!("Success");
			}
			else if(!StdDev[9].is_nan())
			{
				println!("Fail");
			}
			else
			{
				println!("Indeterminate");
			}
		}
	}
}

fn Uniform(rand: u64) -> f64
{
	(rand as f64)/(u64::MAX as f64)*2.0f64-1.0f64
}

#[derive(Clone)]
struct Mersenne_Twist
{
	MT: Vec<u64>,
	index: u64,
}

impl Mersenne_Twist
{
	const w: u64 = 64;
	const n: u64 = 312;
	const m: u64 = 156;
	const r: u64 = 31;
	const a: u64 = 0xb5026f5aa96619e9;
	const u: u64 = 29;
	const d: u128 = 0x5555555555555555;
	const s: u64 = 17;
	const b: u128 = 0x71d67fffeda60000;
	const t: u64 = 37;
	const c: u128 = 0xfff7eee000000000;
	const l: u64 = 43;
	const f: u128 = 6364136223846793005;
	const lower_mask: u64 = (1 << Self::r) - 1;
	const upper_mask: u64 = 0xFFFFFFFFFFFFFFFF & !Self::lower_mask;

	fn new() -> Self
	{
		Self
		{
			MT: Vec::with_capacity(Self::n as usize),
			index: Self::n+1,
		}
	}

	fn srand(&mut self, seed: u64) -> ()
	{
		self.index = Self::n;
		if(self.MT.len() != 0)
		{
			self.MT.clear();
		}

		self.MT.push(seed);
		for i in 1..Self::n
		{
			self.MT.push(0xFFFFFFFFFFFFFFFF & (Self::f*(((self.MT[(i-1) as usize] ^ (self.MT[(i-1) as usize] >> Self::w-2)) + i) as u128)) as u64);
		}
	}

	fn rand(&mut self) -> Option<u64> //Get next random number algorithm
	{
		if(self.MT.len() == 0)
		{
			return(None);
		}
		else if(self.index == Self::n)
		{
			self.twist();
		}

		let mut y: u128 = self.MT[self.index as usize] as u128;
		y = y ^ ((y >> Self::u) & Self::d);
		y = y ^ ((y << Self::s) & Self::b);
		y = y ^ ((y << Self::t) & Self::c);
		y = y ^ (y >> Self::l);

		self.index += 1;
		Some((0xFFFFFFFFFFFFFFFF & y) as u64)
	}

	fn twist(&mut self) -> ()	//Advance the internal RNG state
	{
		for i in 0..Self::n-1
		{
			let x: u64 = (self.MT[i as usize] & Self::upper_mask) | (self.MT[((i+1)%Self::n) as usize] & Self::lower_mask);
			let mut xA: u64 = x >> 1;
			if(x%2 != 0)
			{
				xA = xA ^ Self::a;
			}
			self.MT[i as usize] = self.MT[((i+Self::m)%Self::n) as usize] ^ xA;
		}
		self.index = 0;
	}
}

fn mean(array: &Vec<f64>, N: usize) -> Option<f64>
{
	if(array.len() == 0 || array.len() < N)
	{
		return(None);
	}
	Some(array.iter().sum::<f64>()/(N as f64))
}

fn stddev(array: &Vec<f64>, N: usize) -> Option<f64>
{
	if(array.len() <= 1 || array.len() < N)
	{
		return(None);
	}
	let square_array_sum: f64 = array.iter().map(|&x| x.powi(2)).sum::<f64>();
	let square_mean = mean(array, N).expect("How? This should have returned None before you got here.").powi(2);
	Some((square_array_sum/(N as f64)-square_mean).sqrt())
}

fn meanDev(array: &Vec<f64>, N: usize) -> Option<f64>
{
	if(array.len() == 0 || array.len() < N)
	{
		return(None);
	}
	Some(array.iter().map(|&x| x.powi(2)).sum::<f64>().sqrt()/(N as f64))
}

fn Samples_in_Sphere(array: &Vec<f64>, N: u64) -> u64
{
	let mut In: u64 = 0;

	for vector in array.chunks(N as usize)
	{
		if(vector.iter().map(|&x| x.powi(2)).sum::<f64>().sqrt() < 1.0f64)
		{
			In += 1;
		}
	}

	return(In);
}

#[cfg(test)]
mod tests {
	use super::*;

	#[test]
	fn test_mean_non_empty_vector()
	{
		let input: Vec<f64> = vec![1.0, 2.0, 3.0, 4.0];
		let result: Option<f64> = mean(&input, input.len());
		assert_eq!(result, Some(2.5));
	}

	#[test]
	fn test_mean_empty_vector()
	{
		let input: Vec<f64> = Vec::new();
		let result = mean(&input, input.len());
		assert_eq!(result, None);
	}

	#[test]
	fn test_mean_more_elements_claimed_than_vector()
	{
		let input: Vec<f64> = vec![1.0, 2.0, 3.0, 4.0];
		let result = mean(&input, input.len()+1);
		assert_eq!(result, None);
	}

	#[test]
	fn test_stddev_non_empty_vector()
	{
		let input: Vec<f64> = vec![1.0, 2.0, 3.0, 4.0];
		let result: Option<f64> = stddev(&input);
		assert_eq!(result, Some((1.25 as f64).sqrt()));
	}

	#[test]
	fn test_stddev_one_element_vector()
	{
		let input: Vec<f64> = vec![1.0];
		let result = stddev(&input);
		assert_eq!(result, None);
	}

	#[test]
	fn test_stddev_more_elements_claimed_than_vector()
	{
		let input: Vec<f64> = vec![1.0, 2.0, 3.0, 4.0];
		let result = stddev(&input, input.len()+1);
		assert_eq!(result, None);
	}

	#[test]
	fn test_stddev_empty_vector()
	{
		let input: Vec<f64> = Vec::new();
		let result = stddev(&input);
		assert_eq!(result, None);
	}

	#[test]
	fn test_meanDev_non_empty_vector()
	{
		let input: Vec<f64> = vec![1.0, 2.0, 3.0, 4.0];
		let result: Option<f64> = meanDev(&input);
		assert_eq!(result, Some((30.0 as f64).sqrt()/(4.0 as f64)));
	}

	#[test]
	fn test_meanDev_empty_vector()
	{
		let input: Vec<f64> = Vec::new();
		let result = meanDev(&input);
		assert_eq!(result, None);
	}

	#[test]
	fn test_meanDev_more_elements_claimed_than_vector()
	{
		let input: Vec<f64> = vec![1.0, 2.0, 3.0, 4.0];
		let result = meanDev(&input, input.len()+1);
		assert_eq!(result, None);
	}

	#[test]
	fn test_no_seed()
	{
		let mut RNG: Mersenne_Twist = Mersenne_Twist::new();
		assert_eq!(RNG.rand(), None);
	}

	#[test]
	fn test_no_repeat()
	{
		let mut RNG: Mersenne_Twist = Mersenne_Twist::new();
		RNG.srand(0);
		let first_sample: u64 = RNG.rand().expect("No seed");
		for _ in 0..1000
		{
			assert_ne!(RNG.rand().expect("No seed"), first_sample);
		}
	}
}
