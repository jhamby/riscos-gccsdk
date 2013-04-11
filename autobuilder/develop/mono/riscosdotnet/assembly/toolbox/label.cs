//
// toolbox/label.cs - a binding of the RISC OS API to C#.
//
// Author: Lee Noar (leenoar@sky.com)
//
 
using System;

namespace riscos
{
	public static partial class Toolbox
	{
		/*! \class Label
		 * \brief Consists of some explanation text which appears in a dialogue box.
		 *
		 * The client application can choose whether the bounding box of the label
		 * is shown by a visible box or not.  */
		public class Label : Gadget
		{
			/*! \brief Wrap an existing label, e.g., from a Resource file created
			 * Window.  */
			public Label (Window window, uint cmpID) : base (window, cmpID)
			{
			}
		}

		public static class LabelTemplateOffset
		{
			public const int Label = 36;
			public const int TemplateSize = 40;
		}

		public static class LabelFlags
		{
			public const int NoBoundingBox = (1 << 0);
			public const int Justify = (1 << 1);
		}

		public enum LabelJustify
		{
			Left,
			Right,
			Centre
		}

		/*! \brief Encapsulate the data required to create a Toolbox Label gadget
		 * template.  */
		public class LabelTemplate : GadgetTemplate
		{
			string _label = "";
			//! \brief Get or set the text of the label.
			public string Label {
				get { return _label; }
				set { _label = value; }
			}

			//! \brief Create a Toolbox Label gadget displaying the given text.
			public LabelTemplate (string label) : base (Gadget.ComponentType.Label)
			{
				_label = label;
			}

			/*! \brief Set or get whether the label created from this template will display
			 * a bounding box. It does by default.  */
			public bool HasBoundingBox {
				get { return (_flags & LabelFlags.NoBoundingBox) == 0; }
				set {
				      // Note the logic is reversed here because the flag is set
				      // when there is no bounding box.
				      _flags = (uint)(value ? _flags & ~LabelFlags.NoBoundingBox :
							      _flags |  LabelFlags.NoBoundingBox);
				}
			}

			/*! \brief Set or get the justification of the text in this label.  */
			public LabelJustify Justify {
				get { return (LabelJustify)((_flags >> LabelFlags.Justify) & 3); }
				set {
					_flags &= (3 << LabelFlags.Justify);
					_flags |= (uint)value << LabelFlags.Justify;
				}
			}

			public override int CalculateBufferSize (ref int strStart, ref int msgStart)
			{
				int size = base.CalculateBufferSize (ref strStart, ref msgStart);

				size += _label.Length + 1;

				return size;
			}

			public override void BuildBuffer (IntPtr buffer,
							  int offset,
							  ref int strOffset,
							  ref int msgOffset)
			{
				base.BuildBuffer (buffer, offset, ref strOffset, ref msgOffset);

				msgOffset = ObjectTemplate.WriteString (_label,
									0,
									buffer,
									offset + LabelTemplateOffset.Label,
									msgOffset);
			}

			public override int GetTemplateSize ()
			{
				return LabelTemplateOffset.TemplateSize;
			}
		}
	}
}
