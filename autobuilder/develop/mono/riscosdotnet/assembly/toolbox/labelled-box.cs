//
// toolbox/labelled-box.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//
 
using System;

namespace riscos
{
	public static partial class Toolbox
	{
		/*! \class LabelledBox
		 * \brief Used for collecting together a set of related items.  */
		public class LabelledBox : Gadget
		{
			/*! \brief Wrap an existing labelled box, e.g., from a Resource file created
			 * Window.  */
			public LabelledBox (Window window, uint cmpID) : base (window, cmpID)
			{
			}
		}

		public static class LabelledBoxTemplateOffset
		{
			public const int Label = 36;
			public const int TemplateSize = 40;
		}

		/*! \brief The Toolbox flags that help to define the behaviour of a labelled box.  */
		public static class LabelledBoxFlags
		{
			public const int HasSprite = (1 << 0);
			public const int Filled = (1 << 1);
		}

		/*! \brief Encapsulate the data required to create a Toolbox LabelledBox gadget
		 * template.  */
		public class LabelledBoxTemplate : GadgetTemplate
		{
			string _label = "";
			/*! \brief Either the text of the label or the name of the sprite
			 * to be displayed.  */
			public string Label {
				get { return _label; }
				set { _label = value; }
			}

			/*! \brief Create a Toolbox LabelledBox gadget template with the given label.
			 * This may simply be the text to display or the name of a sprite (in which
			 * case HasSprite must be set to true).  */
			public LabelledBoxTemplate (string label) : base (Gadget.ComponentType.LabelledBox)
			{
				_label = label;
				// User Interface Toolbox Manual Page 367 - this is what ResEd does.
				CreatedFirst = true;
			}

			/*! \brief Get or set whether the label of this labelled box template is a sprite
			 * or text (the default).  */
			public bool HasSprite {
				get { return (_flags & LabelledBoxFlags.HasSprite) != 0; }
				set {
					_flags = (uint)(value ? _flags |  LabelledBoxFlags.HasSprite :
								_flags & ~LabelledBoxFlags.HasSprite);
				}
			}

			/*! \brief Get or set, in the case of a sprite label, whether the icon is filled.  */
			public bool Filled {
				get { return (_flags & LabelledBoxFlags.Filled) != 0; }
				set {
					_flags = (uint)(value ? _flags |  LabelledBoxFlags.Filled :
								_flags & ~LabelledBoxFlags.Filled);
				}
			}

			public override int CalculateBufferSize (ref int strStart, ref int msgStart)
			{
				int size = base.CalculateBufferSize (ref strStart, ref msgStart);
				int length = _label.Length + 1;

				if (HasSprite)
				{
					if (!string.IsNullOrEmpty (_label))
					{
						size += length;
						msgStart += length;
					}
				}
				else if (!string.IsNullOrEmpty (_label))
					size += length;

				return size;
			}

			public override void BuildBuffer (IntPtr buffer,
							  int offset,
							  ref int strOffset,
							  ref int msgOffset)
			{
				base.BuildBuffer (buffer, offset, ref strOffset, ref msgOffset);

				if (HasSprite)
				{
					strOffset = ObjectTemplate.WriteString (_label,
										0,
										buffer,
										offset + LabelledBoxTemplateOffset.Label,
										strOffset);
				}
				else
				{
					msgOffset = ObjectTemplate.WriteString (_label,
										0,
										buffer,
										offset + LabelledBoxTemplateOffset.Label,
										msgOffset);
				}
			}

			public override int GetTemplateSize ()
			{
				return LabelledBoxTemplateOffset.TemplateSize;
			}
		}
	}
}
