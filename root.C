
void root () {
    
    double x=0.0025, y=1253.5;
    std::cout << "Running Macro..." << std::endl;
    std::cout << "x = " << x << std::endl;
    std::cout << "y = " << y << std::endl;
    
    std::cout << "r = sqrt(x^2 + y^2)" << TMath::Sqrt((x*x + y*y)) << std::endl;
    
}
