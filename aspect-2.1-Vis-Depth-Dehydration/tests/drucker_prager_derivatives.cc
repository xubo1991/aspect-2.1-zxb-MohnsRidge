#include <aspect/simulator.h>
#include <deal.II/grid/tria.h>
#include <aspect/material_model/interface.h>
#include <aspect/material_model/drucker_prager.h>
#include <aspect/simulator_access.h>
#include <aspect/newton.h>
#include <aspect/utilities.h>

#include <iostream>

template<int dim>
int f()
{
  std::cout << std::endl << "Test with dimension " << dim << std::endl;

  using namespace aspect::MaterialModel;

  // first set all material model values
  MaterialModelInputs<dim> in_base(5,3);
  in_base.composition[0][0] = 0;
  in_base.composition[0][1] = 0;
  in_base.composition[0][2] = 0;
  in_base.composition[1][0] = 0.75;
  in_base.composition[1][1] = 0.15;
  in_base.composition[1][2] = 0.10;
  in_base.composition[2][0] = 0;
  in_base.composition[2][1] = 0.2;
  in_base.composition[2][2] = 0.4;
  in_base.composition[3][0] = 0;
  in_base.composition[3][1] = 0.2;
  in_base.composition[3][2] = 0.4;
  in_base.composition[4][0] = 1;
  in_base.composition[4][1] = 0;
  in_base.composition[4][2] = 0;

  in_base.temperature[0] = 293;
  in_base.temperature[1] = 1600;
  in_base.temperature[2] = 2000;
  in_base.temperature[3] = 2100;
  in_base.temperature[4] = 2200;

  in_base.pressure[0] = 1e9;
  in_base.pressure[1] = 5e9;
  in_base.pressure[2] = 2e10;
  in_base.pressure[3] = 2e11;
  in_base.pressure[4] = 2e12;

  /**
   * We can't take to small strain-rates, because then the difference in the
   * visocisty will be too small for the double accuracy which stores
   * the visocity solutions and the finite diference solution.
   */
  in_base.strain_rate[0] = SymmetricTensor<2,dim>();
  in_base.strain_rate[0][0][0] = 1e-12;
  in_base.strain_rate[0][0][1] = 1e-12;
  in_base.strain_rate[0][1][1] = 1e-11;
  if (dim == 3)
    {
      in_base.strain_rate[0][2][0] = 1e-12;
      in_base.strain_rate[0][2][1] = 1e-12;
      in_base.strain_rate[0][2][2] = 1e-11;
    }

  in_base.strain_rate[1] = SymmetricTensor<2,dim>(in_base.strain_rate[0]);
  in_base.strain_rate[1][0][0] = -1.71266e-13;
  in_base.strain_rate[1][0][1] = -5.82647e-12;
  in_base.strain_rate[1][1][1] = 4.21668e-14;
  if (dim == 3)
    {
      in_base.strain_rate[1][2][0] = -5.42647e-12;
      in_base.strain_rate[1][2][1] = -5.22647e-12;
      in_base.strain_rate[1][2][2] = 4.21668e-14;
    }
  in_base.strain_rate[2] = SymmetricTensor<2,dim>(in_base.strain_rate[0]);
  in_base.strain_rate[2][1][1] = 1e-13;
  in_base.strain_rate[2][0][1] = 1e-11;
  in_base.strain_rate[2][0][0] = -1e-12;
  if (dim == 3)
    {
      in_base.strain_rate[2][2][0] = 1e-11;
      in_base.strain_rate[2][2][1] = 1e-11;
      in_base.strain_rate[2][2][2] = -1e-12;
    }
  in_base.strain_rate[3] = SymmetricTensor<2,dim>(in_base.strain_rate[0]);
  in_base.strain_rate[3][1][1] = 4.9e-21;
  in_base.strain_rate[3][0][1] = 4.9e-21;
  in_base.strain_rate[3][0][0] = 4.9e-21;
  if (dim == 3)
    {
      in_base.strain_rate[3][2][0] = 4.9e-21;
      in_base.strain_rate[3][2][1] = 4.9e-21;
      in_base.strain_rate[3][2][2] = 4.9e-21;
    }
  in_base.strain_rate[4] = SymmetricTensor<2,dim>(in_base.strain_rate[0]);
  in_base.strain_rate[4][1][1] = 1e-11;
  in_base.strain_rate[4][0][1] = 1e-11;
  in_base.strain_rate[4][0][0] = 1e-11;
  if (dim == 3)
    {
      in_base.strain_rate[4][2][0] = 1e-11;
      in_base.strain_rate[4][2][1] = 1e-11;
      in_base.strain_rate[4][2][2] = 1e-11;
    }

  // initialize some variables we will need later.
  const double finite_difference_accuracy = 1e-7;
  const double finite_difference_factor = 1+finite_difference_accuracy;

  MaterialModelInputs<dim> in_dviscositydstrainrate(in_base);

  MaterialModelOutputs<dim> out_base(5,3);
  MaterialModelOutputs<dim> out_dviscositydpressure(5,3);
  MaterialModelOutputs<dim> out_dviscositydstrainrate(5,3);

  if (out_base.template get_additional_output<MaterialModelDerivatives<dim> >() != nullptr)
    throw "error";

  out_base.additional_outputs.push_back(std::make_shared<MaterialModelDerivatives<dim> > (5));

  // initialize the material we want to test.
  DruckerPrager<dim> mat;
  ParameterHandler prm;
  mat.declare_parameters(prm);

  prm.enter_subsection("Material model");
  {
    prm.enter_subsection ("Drucker Prager");
    {
      prm.enter_subsection ("Viscosity");
      {
        prm.set ("Reference strain rate", "1e-20");
        prm.set ("Angle of internal friction", "30");
      }
      prm.leave_subsection();
    }
    prm.leave_subsection();
  }
  prm.leave_subsection();

  mat.parse_parameters(prm);

  mat.evaluate(in_base, out_base);

  // set up additional output for the derivatives
  MaterialModelDerivatives<dim> *derivatives;
  derivatives = out_base.template get_additional_output<MaterialModelDerivatives<dim> >();
  double temp;

  // have a bool so we know whether the test has succeed or not.
  bool Error = false;


  // test the pressure derivative.
  MaterialModelInputs<dim> in_dviscositydpressure(in_base);
  in_dviscositydpressure.pressure[0] *= finite_difference_factor;
  in_dviscositydpressure.pressure[1] *= finite_difference_factor;
  in_dviscositydpressure.pressure[2] *= finite_difference_factor;
  in_dviscositydpressure.pressure[3] *= finite_difference_factor;
  in_dviscositydpressure.pressure[4] *= finite_difference_factor;

  mat.evaluate(in_dviscositydpressure, out_dviscositydpressure);

  for (unsigned int i = 0; i < 5; i++)
    {
      // prevent division by zero. If it is zero, the test has passed, because or
      // the finite difference and the analytical result match perfectly, or (more
      // likely) the material model in independent of this variable.
      temp = (out_dviscositydpressure.viscosities[i] - out_base.viscosities[i]);
      if (in_base.pressure[i] != 0)
        {
          temp /= (in_base.pressure[i] * finite_difference_accuracy);
        }
      std::cout << "pressure: point = " << i << ", Finite difference = " << temp << ", Analytical derivative = " << derivatives->viscosity_derivative_wrt_pressure[i]  << std::endl;
      if (std::fabs(temp - derivatives->viscosity_derivative_wrt_pressure[i]) > 1e-3 * (std::fabs(temp) + std::fabs(derivatives->viscosity_derivative_wrt_pressure[i])))
        {
          std::cout << "   Error: The derivative of the viscosity to the pressure is too different from the analytical value." << std::endl;
          Error = true;
        }

    }

  // test the strain-rate derivative.
  for (unsigned int component = 0; component < SymmetricTensor<2,dim>::n_independent_components; ++component)
    {
      const TableIndices<2> strain_rate_indices = SymmetricTensor<2,dim>::unrolled_to_component_indices (component);

      for (unsigned int i = 0; i < 5; i++)
        {
          in_dviscositydstrainrate.strain_rate[i] = in_base.strain_rate[i]
                                                    + std::fabs(in_base.strain_rate[i][strain_rate_indices])
                                                    * finite_difference_accuracy
                                                    * aspect::Utilities::nth_basis_for_symmetric_tensors<dim>(component);
        }


      mat.evaluate(in_dviscositydstrainrate, out_dviscositydstrainrate);

      for (unsigned int i = 0; i < 5; i++)
        {
          // prevent division by zero. If it is zero, the test has passed, because or
          // the finite difference and the analytical result match perfectly, or (more
          // likely) the material model in independent of this variable.
          temp = out_dviscositydstrainrate.viscosities[i] - out_base.viscosities[i];
          if (temp != 0)
            {
              temp /= std::fabs(in_dviscositydstrainrate.strain_rate[i][strain_rate_indices]) * finite_difference_accuracy;
            }
          std::cout << "strain-rate: point = " << i << ", component = " << component << ", Finite difference = " << temp << ", Analytical derivative = " << derivatives->viscosity_derivative_wrt_strain_rate[i][strain_rate_indices]  << std::endl;
          if (std::fabs(temp - derivatives->viscosity_derivative_wrt_strain_rate[i][strain_rate_indices]) > 1e-3 * (std::fabs(temp) + std::fabs(derivatives->viscosity_derivative_wrt_strain_rate[i][strain_rate_indices])))
            {
              std::cout << "   Error: The derivative of the viscosity to the strain rate is too different from the analytical value." << std::endl;
              Error = true;
            }

        }

    }

  if (Error)
    {
      std::cout << "Some parts of the test where not succesful." << std::endl;
    }
  else
    {
      std::cout << "OK" << std::endl;
    }

  return 42;
}

int exit_function()
{
  exit(0);
  return 42;
}

// run this function by initializing a global variable by it
// test 2D
int test2d = f<2>(); // Testing min function
// test 3D
int test3d = f<3>(); // Testing min function
// exit
int kl = exit_function();


