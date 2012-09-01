#ifndef GTKMM_KNAPSACK_BACK_END
#define GKTMM_KNAPSACK_BACK_END

#include <iostream>

#include <gtkmm/drawingarea.h>
#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/frame.h>
#include <gtkmm/separator.h>
#include <gtkmm/button.h>
#include <gtkmm/radiobutton.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/label.h>
#include <gtkmm/stock.h>
#include <glibmm/thread.h>

#include <stdlib.h>
#include <cairo.h>
#include <cairoplot/cairoplot.h>

#include <knapsack/eoKnapsack.h>

Glib::StaticMutex mutex = GLIBMM_STATIC_MUTEX_INIT;

class GraphPlot : public Gtk::DrawingArea
{
  public:
    GraphPlot()
      {
        signal_expose_event().connect(sigc::mem_fun(*this, &GraphPlot::on_expose_event), false);
      }

    void push_back(double value)
      {
        data.push_back(value);
      }

    void push(const std::vector<double> values)
      {
        data.clear();
        for (unsigned i = 0; i < values.size(); i++)
            data.push_back(values[i]);
      }

    void clear()
      {
        data.clear();
      }

  protected:
    bool on_expose_event(GdkEventExpose* event)
      {
        Glib::RefPtr<Gdk::Window> window = get_window();
        if (window)
          {
            Gtk::Allocation allocation = get_allocation();
            const int width = allocation.get_width();
            const int height = allocation.get_height();

            cairo_t *context = gdk_cairo_create(window->gobj());
            if (not data.empty())
              {
                double *values = GraphPlot::convert(data);
                cairoplot_draw_axis(context, width, height);
                cairoplot_draw_labels(context, width, height, values, data.size(), 1, 0);
                cairoplot_dot_line(context, width, height, values, data.size(), 0.4, 0.9, 0.4, 0);
                free(values);
              }
            else
              {
                double values[] = { 0 };
                cairoplot_draw_axis(context, width, height);
                cairoplot_draw_labels(context, width, height, values, 1, 1, 0);
              }
            cairo_destroy(context);
          }
        return true;
      }

    static double *convert(std::vector<double> data)
      {
        double *values = (double *) calloc(data.size(), sizeof(double));
        for (unsigned i = 0; i < data.size(); i++)
            values[i] = (double) data[i];
        return values;
      }

  private:
    std::vector<double> data;
};

class KnapsackBackEnd : public Gtk::Window
{
  public:
    KnapsackBackEnd() : open_button("Open instance"), start_button("Start"), clear_button("Clear graph"), stop_button("Stop"),
        button_container_left(Gtk::BUTTONBOX_START, 5), button_container_middle(Gtk::BUTTONBOX_START, 5),
        button_container_right(Gtk::BUTTONBOX_START, 5), algo_easy("EasyEA"), algo_ivf("IVF-GA"), running_thread(true),
        info("Population: 500, Generation: 500, Mutation rate: 0.015, Crossover rate: 0.75", 0, 0)
      {
        algorithm = NULL; problem = NULL;

        set_title("Knapsack: Statistics");
        set_position(Gtk::WIN_POS_CENTER);
        set_resizable(false);
        set_border_width(5);

        thread = Glib::Thread::create(sigc::mem_fun(*this,&KnapsackBackEnd::thread_worker),true);
        Glib::signal_timeout().connect(sigc::mem_fun(*this, &KnapsackBackEnd::on_redraw), 100);

        open_button.set_size_request(120, 30);
        open_button.signal_clicked().connect(sigc::mem_fun(*this,
            &KnapsackBackEnd::on_open_button_clicked));

        start_button.set_size_request(70, 30);
        start_button.set_sensitive(false);
        start_button.signal_clicked().connect(sigc::mem_fun(*this,
            &KnapsackBackEnd::on_start_button_clicked));

        clear_button.set_size_request(120, 30);
        clear_button.set_sensitive(false);
        clear_button.signal_clicked().connect(sigc::mem_fun(*this,
            &KnapsackBackEnd::on_clear_button_clicked));

        stop_button.set_size_request(70, 30);
        stop_button.set_sensitive(false);
        stop_button.signal_clicked().connect(sigc::mem_fun(*this,
            &KnapsackBackEnd::on_stop_button_clicked));

        Gtk::RadioButton::Group group = algo_easy.get_group();
        algo_ivf.set_group(group);
        algo_ivf.set_active();

        plot.set_size_request(1000, 500);

        plot_frame.set_label("Graph");
        plot_frame.set_shadow_type(Gtk::SHADOW_ETCHED_IN);
        plot_frame.set_border_width(5);
        plot_frame.add(plot);

        button_container_left.pack_start(open_button);
        button_container_middle.pack_start(start_button);
        button_container_middle.pack_start(clear_button);
        button_container_middle.pack_start(stop_button);
        button_container_right.pack_start(algo_ivf);
        button_container_right.pack_start(algo_easy);

        top_container.set_border_width(5);
        top_container.pack_start(button_container_left);
        top_container.pack_start(button_container_middle);
        top_container.pack_start(button_container_right);

        label_container.set_border_width(5);
        label_container.add(info);

        container.set_border_width(10);
        container.add(top_container);
        container.add(plot_frame);
        container.add(label_container);
        add(container);

        show_all_children();
      }

    virtual ~KnapsackBackEnd()
      {
        {
          Glib::Mutex::Lock lock(mutex);
          if (running_thread)
            running_thread = false;
        }
        if(thread->joinable())
            thread->join();
        delete problem;
        delete algorithm;
      }

  protected:
    Gtk::VBox container;
    Gtk::HBox top_container, label_container;
    Gtk::HButtonBox button_container_left, button_container_middle, button_container_right;
    Gtk::Button open_button, start_button, clear_button, stop_button;
    Gtk::RadioButton algo_easy, algo_ivf;
    Gtk::Frame plot_frame;
    Gtk::Label info;

    GraphPlot plot;
    KnapsackProblem<Individual> *problem;
    Algorithm<Individual> *algorithm;

    bool running_thread;
    Glib::Thread * thread;

    void thread_worker()
      {
        while(true)
          {
            if (algorithm != NULL)
              {
                (*algorithm)();
                if (algorithm != NULL)
                  {
                    delete algorithm;
                    algorithm = NULL;
                  }
                open_button.set_sensitive(true);
                start_button.set_sensitive(true);
                /* FIXME Stop button is not working
                stop_button.set_sensitive(false); */
              }
            {
              Glib::Mutex::Lock lock(mutex);
              if (not running_thread)
                return;
            }
          }
      }

    void on_open_button_clicked()
      {
        Gtk::FileChooserDialog dialog("Please choose a instance file", Gtk::FILE_CHOOSER_ACTION_OPEN);
        dialog.set_transient_for(*this);
        dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
        dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

        Gtk::FileFilter filter_text;
        filter_text.set_name("Text files");
        filter_text.add_mime_type("text/plain");
        dialog.add_filter(filter_text);

        Gtk::FileFilter filter_any;
        filter_any.set_name("Any");
        filter_any.add_mime_type("*");
        dialog.add_filter(filter_any);

        int result = dialog.run();

        if (result == Gtk::RESPONSE_OK)
          {
            std::ifstream input;
            const char *filename = ((std::string) dialog.get_filename()).c_str();
            input.open(filename, std::ifstream::in);
            if (problem != NULL)
              {
                delete problem;
                problem = NULL;
              }
            problem = new KnapsackProblem<Individual>(input);
            input.close();
            start_button.set_sensitive(true);
            clear_button.set_sensitive(true);
            /* FIXME Stop button is not working
            stop_button.set_sensitive(false); */
          }
      }

    void on_start_button_clicked()
      {
        if (algorithm != NULL)
          {
            delete algorithm;
            algorithm = NULL;
          }
        plot.clear();
        if (algo_ivf.get_active())
            algorithm = new AlgorithmIVFGA<Individual>(*problem);
        else
            algorithm = new AlgorithmEasyEA<Individual>(*problem);
        open_button.set_sensitive(false);
        start_button.set_sensitive(false);
        /* FIXME Stop button is not working
        stop_button.set_sensitive(true); */
      }

    void on_stop_button_clicked()
      {
        /* FIXME It's not working at all
        if (algorithm != NULL)
          {
            delete algorithm;
            algorithm = NULL;
          }
        start_button.set_sensitive(true);
        stop_button.set_sensitive(false); */
      }

    void on_clear_button_clicked()
      {
        plot.clear();
      }

    bool on_redraw()
      {
        if (algorithm != NULL)
            if ((*algorithm).statistical_data().best_values().size() > 0)
              {
                unsigned size = (*algorithm).statistical_data().best_values().size();
                double value = (double) (*algorithm).statistical_data().best_values()[size-1];
                plot.push_back(value);
              }
        queue_draw();
        return true;
      }
};

#endif

