
#include <iostream>
#include <vector>
#include <string>

#include <dlib/clustering.h>
#include <dlib/rand.h>

using namespace std;
using namespace dlib;

int main(int argc, char **argv)
{
  if ( argc != 2 ) return 0;
  int number_klasters = atoi(argv[1]);
  // Here we declare that our samples will be 2 dimensional column vectors.  
  // (Note that if you don't know the dimensionality of your vectors at compile time
  // you can change the 2 to a 0 and then set the size at runtime)
  typedef matrix<double, 2, 1> sample_type;

  // Now we are making a typedef for the kind of kernel we want to use.  I picked the
  // radial basis kernel because it only has one parameter and generally gives good
  // results without much fiddling.
  typedef radial_basis_kernel<sample_type> kernel_type;

  sample_type d;
  std::vector<sample_type> values_to_predict;
  std::string s;
  while ( std::getline(std::cin, s) ) {
    std::size_t pos = s.find(';');
    if ( pos != std::string::npos ) {
      //cout << s.substr(0, pos) << endl;
      d(0) = stod(s.substr(0, pos));
      d(1) = stod(s.substr(pos + 1, s.length()));
      values_to_predict.push_back(d);
    }
  }

  // Here we declare an instance of the kcentroid object.  It is the object used to 
  // represent each of the centers used for clustering.  The kcentroid has 3 parameters 
  // you need to set.  The first argument to the constructor is the kernel we wish to 
  // use.  The second is a parameter that determines the numerical accuracy with which 
  // the object will perform part of the learning algorithm.  Generally, smaller values 
  // give better results but cause the algorithm to attempt to use more dictionary vectors 
  // (and thus run slower and use more memory).  The third argument, however, is the 
  // maximum number of dictionary vectors a kcentroid is allowed to use.  So you can use
  // it to control the runtime complexity.  
  kcentroid<kernel_type> kc(kernel_type(0.1),0.01, 8);

  // Now we make an instance of the kkmeans object and tell it to use kcentroid objects
  // that are configured with the parameters from the kc object we defined above.
  kkmeans<kernel_type> test(kc);

  std::vector<sample_type> samples;
  std::vector<sample_type> initial_centers;

  sample_type m;

  dlib::rand rnd;

  // we will make 50 points from each class
  const long num = 50;

  // make some samples near the origin
  double radius = 0.5;
  for (long i = 0; i < num; ++i)
  {
      double sign = 1;
      if (rnd.get_random_double() < 0.5)
          sign = -1;
      m(0) = 2*radius*rnd.get_random_double()-radius;
      m(1) = sign*sqrt(radius*radius - m(0)*m(0));

      // add this sample to our set of samples we will run k-means 
      samples.push_back(m);
  }

  // make some samples in a circle around the origin but far away
  radius = 10.0;
  for (long i = 0; i < num; ++i)
  {
      double sign = 1;
      if (rnd.get_random_double() < 0.5)
          sign = -1;
      m(0) = 2*radius*rnd.get_random_double()-radius;
      m(1) = sign*sqrt(radius*radius - m(0)*m(0));

      // add this sample to our set of samples we will run k-means 
      samples.push_back(m);
  }

  // make some samples in a circle around the point (25,25) 
  radius = 4.0;
  for (long i = 0; i < num; ++i)
  {
      double sign = 1;
      if (rnd.get_random_double() < 0.5)
          sign = -1;
      m(0) = 2*radius*rnd.get_random_double()-radius;
      m(1) = sign*sqrt(radius*radius - m(0)*m(0));

      // translate this point away from the origin
      m(0) += 25;
      m(1) += 25;

      // add this sample to our set of samples we will run k-means 
      samples.push_back(m);
  }

  // tell the kkmeans object we made that we want to run k-means with k set to 3. 
  // (i.e. we want 3 clusters)
  test.set_number_of_centers(number_klasters);

  // You need to pick some initial centers for the k-means algorithm.  So here
  // we will use the dlib::pick_initial_centers() function which tries to find
  // n points that are far apart (basically).  
  pick_initial_centers(number_klasters, initial_centers, samples, test.get_kernel());

  // now run the k-means algorithm on our set of samples.  
  test.train(samples,initial_centers);

  
  // выводим 
  for ( const sample_type& value : values_to_predict ) {
    cout << value(0) << ";" << value(1) << ";" << test(value) << "\n";
  }
	return 0;
}