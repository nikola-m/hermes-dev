// This file is part of Hermes2D.
//
// Hermes2D is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// Hermes2D is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Hermes2D.  If not, see <http://www.gnu.org/licenses/>.

#ifndef __H2D_H1_WEAK_FORMS_H
#define __H2D_H1_WEAK_FORMS_H

#include "../integrals/integrals_h1.h"

/* Default volumetric matrix form \int_{area} coeff \nabla u \cdot \nabla v d\bfx 
   coeff... constant number
*/

namespace WeakFormsH1 {
  namespace VolumetricMatrixForms {
    class DefaultMatrixFormGradGrad : public WeakForm::MatrixFormVol
    {
    public:
      DefaultMatrixFormGradGrad(int i, int j, double coeff = 1.0, SymFlag sym = HERMES_SYM) 
            : WeakForm::MatrixFormVol(i, j, sym), coeff(coeff) { }
      DefaultMatrixFormGradGrad(int i, int j, std::string area, 
                                 double coeff = 1.0, SymFlag sym = HERMES_SYM) 
            : WeakForm::MatrixFormVol(i, j, sym, area), coeff(coeff) { }

      template<typename Real, typename Scalar>
      Scalar matrix_form(int n, double *wt, Func<Scalar> *u_ext[], Func<Real> *u, 
                         Func<Real> *v, Geom<Real> *e, ExtData<Scalar> *ext) {
        return coeff * int_grad_u_grad_v<Real, Scalar>(n, wt, u, v);
      }

      scalar value(int n, double *wt, Func<scalar> *u_ext[], Func<double> *u, 
                   Func<double> *v, Geom<double> *e, ExtData<scalar> *ext) {
        return matrix_form<scalar, scalar>(n, wt, u_ext, u, v, e, ext);
      }

      Ord ord(int n, double *wt, Func<Ord> *u_ext[], Func<Ord> *u, Func<Ord> *v, 
              Geom<Ord> *e, ExtData<Ord> *ext) {
        return matrix_form<Ord, Ord>(n, wt, u_ext, u, v, e, ext);
      }

      private:
        double coeff;
    };

    /* Default volumetric matrix form \int_{area} coeff u v d\bfx 
       coeff... constant number
    */

    class DefaultMatrixFormMass : public WeakForm::MatrixFormVol
    {
    public:
      DefaultMatrixFormMass(int i, int j, double coeff = 1.0, SymFlag sym = HERMES_SYM) 
            : WeakForm::MatrixFormVol(i, j, sym), coeff(coeff) { }
      DefaultMatrixFormMass(int i, int j, std::string area, double coeff = 1.0, SymFlag sym = HERMES_SYM) 
            : WeakForm::MatrixFormVol(i, j, sym, area), coeff(coeff) { }

      template<typename Real, typename Scalar>
      Scalar matrix_form(int n, double *wt, Func<Scalar> *u_ext[], Func<Real> *u, 
                         Func<Real> *v, Geom<Real> *e, ExtData<Scalar> *ext) {
        return coeff * int_u_v<Real, Scalar>(n, wt, u, v);
      }

      scalar value(int n, double *wt, Func<scalar> *u_ext[], Func<double> *u, 
                   Func<double> *v, Geom<double> *e, ExtData<scalar> *ext) {
        return matrix_form<scalar, scalar>(n, wt, u_ext, u, v, e, ext);
      }

      Ord ord(int n, double *wt, Func<Ord> *u_ext[], Func<Ord> *u, Func<Ord> *v, 
              Geom<Ord> *e, ExtData<Ord> *ext) {
        return matrix_form<Ord, Ord>(n, wt, u_ext, u, v, e, ext);
      }

      private:
        double coeff;
    };

    /* Default volumetric matrix form \int_{area} (coeff1, coeff2) \cdot \nabla u vd\bfx 
       coeff1, coeff2... constant number
    */

    class DefaultMatrixFormAdvection : public WeakForm::MatrixFormVol
    {
    public:
     DefaultMatrixFormAdvection(int i, int j, double coeff1, double coeff2) 
       : WeakForm::MatrixFormVol(i, j, HERMES_NONSYM), coeff1(coeff1), coeff2(coeff2) { }
     DefaultMatrixFormAdvection(int i, int j, std::string area, double coeff1, double coeff2) 
       : WeakForm::MatrixFormVol(i, j, HERMES_NONSYM, area), coeff1(coeff1), coeff2(coeff2) { }

      template<typename Real, typename Scalar>
      Scalar matrix_form(int n, double *wt, Func<Scalar> *u_ext[], Func<Real> *u, 
                         Func<Real> *v, Geom<Real> *e, ExtData<Scalar> *ext) {
        return   coeff1 * int_dudx_v<Real, Scalar>(n, wt, u, v)
               + coeff2 * int_dudy_v<Real, Scalar>(n, wt, u, v);
      }

      scalar value(int n, double *wt, Func<scalar> *u_ext[], Func<double> *u, 
                   Func<double> *v, Geom<double> *e, ExtData<scalar> *ext) {
        return matrix_form<scalar, scalar>(n, wt, u_ext, u, v, e, ext);
      }

      Ord ord(int n, double *wt, Func<Ord> *u_ext[], Func<Ord> *u, Func<Ord> *v, 
              Geom<Ord> *e, ExtData<Ord> *ext) {
        return matrix_form<Ord, Ord>(n, wt, u_ext, u, v, e, ext);
      }

      private:
      double coeff1, coeff2;
    };
  }

  namespace RightHandSides {
    // Generic class for non-constant right-hand side. 
    class DefaultNonConstRightHandSide
    {
    public:
      DefaultNonConstRightHandSide() { };

      virtual scalar value(double x, double y) const = 0;
      virtual Ord ord(Ord x, Ord y) const = 0;
    };
  }

  namespace VolumetricVectorForms {
      /* Default volumetric vector form \int_{area} coeff v d\bfx 
         coeff... constant number
      */

      class DefaultVectorFormConst : public WeakForm::VectorFormVol
      {
      public:
        DefaultVectorFormConst(int i, double coeff) 
                     : WeakForm::VectorFormVol(i), coeff(coeff) { }
        DefaultVectorFormConst(int i, std::string area, double coeff) 
                     : WeakForm::VectorFormVol(i, area), coeff(coeff) { }

        virtual scalar value(int n, double *wt, Func<scalar> *u_ext[], Func<double> *v,
                             Geom<double> *e, ExtData<scalar> *ext) {
          return coeff * int_v<scalar, scalar>(n, wt, v);
        }

        virtual Ord ord(int n, double *wt, Func<Ord> *u_ext[], Func<Ord> *v,
                Geom<Ord> *e, ExtData<Ord> *ext) {
          return int_v<Ord, Ord>(n, wt, v);
        }

      private:
        double coeff;
      };

      /* Default volumetric vector form \int_{area} rhs(x, y) v d\bfx 
         rhs(x, y)... non-constant right-hand side
      */
      class DefaultVectorFormNonConst : public WeakForm::VectorFormVol
      {
      public:
        DefaultVectorFormNonConst(int i, RightHandSides::DefaultNonConstRightHandSide* rhs) 
                     : WeakForm::VectorFormVol(i), rhs(rhs) { }
        DefaultVectorFormNonConst(int i, std::string area, RightHandSides::DefaultNonConstRightHandSide* rhs) 
                     : WeakForm::VectorFormVol(i, area), rhs(rhs) { }

        scalar value(int n, double *wt, Func<scalar> *u_ext[], Func<double> *v,
                     Geom<double> *e, ExtData<scalar> *ext) {
          scalar result = 0;
          for (int i = 0; i < n; i++)
            result += wt[i] * (rhs->value(e->x[i], e->y[i]) * v->val[i]);
          return result;
        }

        Ord ord(int n, double *wt, Func<Ord> *u_ext[], Func<Ord> *v,
                Geom<Ord> *e, ExtData<Ord> *ext) {
          Ord result = 0;
          for (int i = 0; i < n; i++)
            result += wt[i] * (rhs->ord(e->x[i], e->y[i]) * v->val[i]);
          return result;
        }

      private:
        RightHandSides::DefaultNonConstRightHandSide* rhs;
      };
  }

  namespace SurfaceMatrixForms {
    /* Default surface matrix form \int_{area} coeff u v dS
       coeff... constant number
    */

    class DefaultSurfaceMatrixForm : public WeakForm::MatrixFormSurf
    {
    public:
      DefaultSurfaceMatrixForm(int i, int j, double coeff) 
            : WeakForm::MatrixFormSurf(i, j), coeff(coeff) { }
      DefaultSurfaceMatrixForm(int i, int j, std::string area, double coeff) 
            : WeakForm::MatrixFormSurf(i, j, area), coeff(coeff) { }

      template<typename Real, typename Scalar>
      Scalar matrix_form_surf(int n, double *wt, Func<Scalar> *u_ext[], Func<Real> *u, 
                              Func<Real> *v, Geom<Real> *e, ExtData<Scalar> *ext) {
        return coeff * int_u_v<Real, Scalar>(n, wt, u, v);
      }

      scalar value(int n, double *wt, Func<scalar> *u_ext[], Func<double> *u, Func<double> *v, 
                   Geom<double> *e, ExtData<scalar> *ext) {
        return matrix_form_surf<scalar, scalar>(n, wt, u_ext, u, v, e, ext);
      }

      Ord ord(int n, double *wt, Func<Ord> *u_ext[], Func<Ord> *u, 
              Func<Ord> *v, Geom<Ord> *e, ExtData<Ord> *ext) {
        return matrix_form_surf<Ord, Ord>(n, wt, u_ext, u, v, e, ext);
      }

      private:
        double coeff;
    };
  }

  namespace SurfaceVectorForms {
    /* Default surface vector form \int_{area} coeff v dS
       coeff... constant number
    */

    class DefaultSurfaceVectorForm : public WeakForm::VectorFormSurf
    {
    public:
      DefaultSurfaceVectorForm(int i, double coeff) 
             : WeakForm::VectorFormSurf(i), coeff(coeff) { }
      DefaultSurfaceVectorForm(int i, std::string area, double coeff) 
             : WeakForm::VectorFormSurf(i, area), coeff(coeff) { }

      template<typename Real, typename Scalar>
      Scalar vector_form_surf(int n, double *wt, Func<Scalar> *u_ext[], 
                              Func<Real> *v, Geom<Real> *e, ExtData<Scalar> *ext) {
        return coeff * int_v<Real, Scalar>(n, wt, v);
      }

      scalar value(int n, double *wt, Func<scalar> *u_ext[], Func<double> *v, 
                   Geom<double> *e, ExtData<scalar> *ext) {
        return vector_form_surf<scalar, scalar>(n, wt, u_ext, v, e, ext);
      }

      Ord ord(int n, double *wt, Func<Ord> *u_ext[], Func<Ord> *v, Geom<Ord> *e, ExtData<Ord> *ext) {
        return vector_form_surf<Ord, Ord>(n, wt, u_ext, v, e, ext);
      }

    private:
      double coeff;
    };
  }

  namespace WeakForms {
    /* Default weak form for the Laplace equation -Laplace u = 0
    */

    class DefaultWeakFormLaplace : public WeakForm
    {
    public:
      DefaultWeakFormLaplace() : WeakForm(1)
      {
        add_matrix_form(new VolumetricMatrixForms::DefaultMatrixFormGradGrad(0, 0));
      };
    };
  }
}
#endif