public class Complex
{
	private double real;
	private double imag;

	public Complex(double real, double imag)
	{
		this.real = real;
		this.imag = imag;
	}

	public double getReal()
	{return(real);}

	public double getImag()
	{return(imag);}

	public void setReal(double real)
	{this.real = real;}

	public void setImag(double imag)
	{this.imag = imag;}

	public Complex square()
	{return(new Complex(Math.pow(real,2)-Math.pow(imag,2),2.0*real*imag));}

	public double abs()
	{return(Math.sqrt(Math.pow(real,2)+Math.pow(imag,2)));}

	public Complex add(Complex a)
	{return(new Complex(real+a.real,imag+a.imag));}
}
