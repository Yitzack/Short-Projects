#![allow(clippy::needless_bool)]
#![allow(unused_parens)]
#![allow(non_snake_case)]
#![allow(non_camel_case_types)]
#![allow(non_upper_case_globals)]

use std::time::{SystemTime, UNIX_EPOCH};
use std::f64::consts::PI;

fn main()
{
	let mut Samples_Used: Vec<u64> = Vec::with_capacity(9);
	let mut Points_in_Sphere: Vec<u64> = Vec::with_capacity(9);
	let mut Sample: Vec<f64> = Vec::with_capacity(2520);
	let Sub_Samples: Vec<u16> = vec![1260,840,630,504,420,360,315,280,252];
	let mut pi: Vec<f64> = vec![0.0f64; 9];
	let mut Mean: f64;
	let mut StdDev: f64;

	let mut RNG: Mersenne_Twist = Mersenne_Twist::new();
	RNG.srand(SystemTime::now().duration_since(UNIX_EPOCH).expect("System Error").as_secs());

	for _ in 0..9
	{
		Samples_Used.push(0);
		Points_in_Sphere.push(0);
	}

	for _ in 0..2520
	{
		Sample.push(Uniform(RNG.rand().expect("Unseeded RNG")));
	}

	loop
	{
		for N in 2..=10
		{
			Points_in_Sphere[N-2] += Samples_in_Sphere(&Sample, N as u64);
			Samples_Used[N-2] += Sub_Samples[N-2] as u64;
		}

		if((Samples_Used[8]/252)%1000==999)
		{
			pi[0] = 4.0f64*(Points_in_Sphere[0] as f64)/(Samples_Used[0] as f64);
			pi[1] = 6.0f64*(Points_in_Sphere[1] as f64)/(Samples_Used[1] as f64);
			pi[2] = 4.0f64*(2.0f64*(Points_in_Sphere[2] as f64)/(Samples_Used[2] as f64)).sqrt();
			pi[3] = 2.0f64*(15.0f64*(Points_in_Sphere[3] as f64)/(Samples_Used[3] as f64)).sqrt();
			pi[4] = 4.0f64*(6.0f64*(Points_in_Sphere[4] as f64)/(Samples_Used[4] as f64)).powf(1.0f64/3.0f64);
			pi[5] = 2.0f64*(105.0f64*(Points_in_Sphere[5] as f64)/(Samples_Used[5] as f64)).powf(1.0f64/3.0f64);
			pi[6] = 4.0f64*(24.0f64*(Points_in_Sphere[6] as f64)/(Samples_Used[6] as f64)).powf(0.25f64);
			pi[7] = 2.0f64*(945.0f64*(Points_in_Sphere[7] as f64)/(Samples_Used[7] as f64)).powf(0.25f64);
			pi[8] = 4.0f64*(120.0f64*(Points_in_Sphere[8] as f64)/(Samples_Used[8] as f64)).powf(0.2f64);

			Mean = mean(&pi).expect("Insufficent data");
			StdDev = stddev(&pi).expect("Insufficent data");

			print!("{},{},{},{},{},{},{},{},{},{},Around[{},{}],",Samples_Used[8]+1,pi[0],pi[1],pi[2],pi[3],pi[4],pi[5],pi[6],pi[7],pi[8],Mean,StdDev);

			if(Mean-StdDev < PI && Mean+StdDev > PI)
			{
				println!("Success");
			}
			else if(!StdDev.is_nan())
			{
				println!("Fail");
			}
			else
			{
				println!("Indeterminate");
			}
		}

		for element in Sample.iter_mut()
		{
			*element = Uniform(RNG.rand().expect("Unseeded RNG"));
		}
	}
}

fn Uniform(rand: u64) -> f64
{
	(rand as f64)/(u64::MAX as f64)*2.0f64-1.0f64
}

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

fn mean(array: &Vec<f64>) -> Option<f64>
{
	if(array.len() == 0)
	{
		return(None);
	}
	Some(array.iter().sum::<f64>()/(array.len() as f64))
}

fn stddev(array: &Vec<f64>) -> Option<f64>
{
	if(array.len() <= 1)
	{
		return(None);
	}
	let square_array_sum: f64 = array.iter().map(|&x| x.powi(2)).sum::<f64>();
	let square_mean = mean(array).expect("How? This should have returned None before you got here.").powi(2);
	Some((square_array_sum/(array.len() as f64)-square_mean).sqrt())
}

fn meanDev(array: &Vec<f64>) -> Option<f64>
{
	if(array.len() == 0)
	{
		return(None);
	}
	Some(array.iter().map(|&x| x.powi(2)).sum::<f64>().sqrt()/(array.len() as f64))
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
		let result: Option<f64> = mean(&input);
		assert_eq!(result, Some(2.5));
	}

	#[test]
	fn test_mean_empty_vector()
	{
		let input: Vec<f64> = Vec::new();
		let result = mean(&input);
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
