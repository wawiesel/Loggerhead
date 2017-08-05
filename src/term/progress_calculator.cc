#include "progress_calculator.hh"
#include <iostream>

namespace term
{
progress_calculator::progress_calculator( int total_tasks )
{
    reset( total_tasks );
}

void progress_calculator::update( int dtasks )
{
    if( b_is_finished ) return;

    // delta tasks
    if( dtasks < 0 )
    {
        b_completed_tasks -= dtasks;
    }
    // absolute number of completed tasks
    else
    {
        b_completed_tasks = dtasks;
    }
    if( b_completed_tasks >= b_total_tasks ) finish();
}

void progress_calculator::finish()
{
    if( !b_is_finished )
    {
        b_completed_tasks = b_total_tasks;
        b_final_elapsed = elapsed_seconds();
        b_is_finished = true;
    }
}

void progress_calculator::reset( int total_tasks )
{
    b_total_tasks = total_tasks;
    b_completed_tasks = 0;
    b_is_finished = false;
    b_start = std::chrono::system_clock::now();
    b_final_elapsed = 0.0;
}

void progress_calculator::set_total_tasks( int total_tasks )
{
    b_total_tasks = total_tasks;
    if( b_completed_tasks >= b_total_tasks ) finish();
}

double progress_calculator::elapsed_seconds() const
{
    if( b_is_finished ) return b_final_elapsed;
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed = end - b_start;
    return elapsed.count();
}

double progress_calculator::remaining_seconds() const
{
    double frac = fraction_completed();
    double elapsed = elapsed_seconds();
    if( elapsed == 0.0 )elapsed = 3600.;

    double remain = ( 1.0 / frac - 1.0 ) * elapsed;
    if( remain < 0.0 ) remain = 0.0;
    return remain;
}

int progress_calculator::remaining_tasks() const
{
    return b_total_tasks - b_completed_tasks;
}

int progress_calculator::total_tasks() const { return b_total_tasks; }
int progress_calculator::completed_tasks() const { return b_completed_tasks; }
double progress_calculator::fraction_completed() const
{
    if( b_is_finished ) return 1.0;
    if( b_completed_tasks == 0 || b_total_tasks == 0 ) return 0.0;
    return ( 0.0 + b_completed_tasks ) / ( 0.0 + b_total_tasks );
}

bool progress_calculator::is_finished() const { return b_is_finished; }
}  // progress
